#include "blob.h"

Blob::Blob (const CreateBlob& params) :
	  _name (params._name)
	, _speed (params._speed)
	, _runningSpeed (params._runningSpeed)
	, _baseSmell (params._smell)
	, _baseHP (params._HP)
	, _endurance (params._endurance)
	, _aggression (params._aggression)
	, _lifespan (params._lifespan)
	, _baseDamage (params._damage)
	, _maxHunger (params._maxHunger)
	, _size (params._size)
	, _moveDirectionFn (params._moveDirectionFn)
	, _aggressionFn (params._aggressionFn)
	,  _state ("newborn")
 	, _fatigue (0)
	, _tired (false)
	, _age (0)
	, _hunger (0.0)
	, _dead (false)
{
	_points.push_back (params._position);
	setHP (maxHP ());
}

std::string Blob::name () const
{
	return _name;
}

double Blob::x () const
{
	return _points.back ().x ();
}

double Blob::y () const
{
	return _points.back ().y ();
}

double Blob::baseSpeed () const
{
	return _speed;
}

double Blob::speed () const
{
	return (baseHP () == 0) ? 0U : _speed * (double (_HP)) / baseHP ();
}

double Blob::runningSpeed () const
{
	return (baseHP () == 0) ? 0U :_runningSpeed * (double (_HP)) / baseHP ();
}

double Blob::smell () const
{
	return _baseSmell * ageRatio ();
}

unsigned int Blob::baseHP () const
{
	return _baseHP;
}

unsigned int Blob::HP () const
{
	return _HP;
}

unsigned int Blob::maxHP () const
{
	return ((unsigned int) ((_baseHP * hungerRatio () * ageRatio ()) + 0.5));
}

unsigned int Blob::baseDamage () const
{
	return _baseDamage;
}

unsigned int Blob::damage () const
{
	return (baseHP () == 0) ? 0U : ((unsigned int) ((_baseDamage * (double (_HP)) / baseHP ()) + 0.5));
}

unsigned int Blob::endurance () const
{
	return _endurance;
}

double Blob::aggression () const
{
	return isDead () ? _aggression : ((2.0 - hungerRatio ()) * _aggression);
}

unsigned int Blob::maxHunger () const
{
	return _maxHunger;
}

double Blob::hunger () const
{
	return _hunger;
}

unsigned int Blob::size () const
{
	return ((unsigned int) ((((double) _size) * ageRatio ()) + 0.5)) ;
}

unsigned int Blob::lifespan () const
{
	return _lifespan;
}

unsigned int Blob::age () const
{
	return _age;
}

double Blob::ageRatio () const
{
	double a = -2.0 / (((double) _lifespan) * _lifespan);
	double b = 2.0 / ((double) _lifespan);
	double c = 0.5;

	return a * ((double) _age) * _age + b * ((double) _age) + c;
}

double Blob::hungerRatio () const
{
	return (_maxHunger - _hunger) / _maxHunger;
}

std::string Blob::state () const
{
	return _state;
}

double Blob::fatigue () const
{
	return _fatigue;
}

bool Blob::isTired () const
{
	return _tired;
}
 

bool Blob::isDead () const
{
	return _dead;
}


const std::vector<Pt<double>>& Blob::history () const
{
	return _points;
}

double Blob::distance (const Blob& other) const
{
	double dx = other.x () - x();
	double dy = other.y () - y();
	double d = sqrt ((dx * dx) + (dy * dy));

	return d; 
}

bool Blob::isInRange (const Blob& other, double range) const
{
	return distance (other) <= range;
}

bool Blob::isInSameSquare (const Blob& other) const
{
	return isInRange (other, sqrt (2));
}

bool Blob::canSmell (const Blob& other) const
{
	return isInRange (other, smell ());
}

double Blob::angle (const Blob& other) const
{
	double dx = x () - other.x ();
	double dy = y () - other.y ();

	return atan2 (dy, -dx) + M_PI/2;
}

void Blob::kill ()
{
	_dead = true;
	_state = "dead";
	_speed = 0.0;
	_runningSpeed = 0.0;
	_baseSmell = 0.0;
	_baseHP = 0U;
	_HP = 0U;
	_baseDamage = 0U;
	_endurance = 0U;
	_aggression = 0.0;
	_maxHunger = 0U;
	_hunger = 0.0;
}

void Blob::setHP (unsigned int newHP)
{
	_HP = newHP;
	if (_HP == 0U)
	{
		kill ();
	}
}

void Blob::limitHPtoMax (unsigned int previousDamage)
{
	// this is so that if we increase max (due to aging), then our current HP go up by the same amount
	setHP (maxHP () - previousDamage);
}

void Blob::growOlder ()
{
	if (!isDead ())
	{
		unsigned int previousDamage = maxHP () - HP ();
		if (++_age >= lifespan ())
		{
			kill ();
		}	
		limitHPtoMax (previousDamage);
		if (_HP < maxHP ())
		{
			setHP (_HP + 1U);
		}
	}
}

void Blob::getHungrier (double amount)
{
	unsigned int previousDamage = maxHP () - HP ();
	_hunger += amount;
	if (_hunger > (double) _maxHunger)
	{
		_hunger = (double) _maxHunger;
	}
	limitHPtoMax (previousDamage);
}

void Blob::takeDamage (unsigned int damage)
{
	if (_HP >= damage)
	{
		setHP (_HP - damage);
	}
	else
	{
		setHP (0);
	}
}

void Blob::inflictDamage (Target* target, const std::string& state)
{
	_state = _state;
	growOlder ();
	target->takeDamage (damage ());
	getHungrier (damage ());
}

void Blob::retaliate (Target* target)
{
	target->takeDamage (damage ());
}

void Blob::move (double speed, double angleInRadians, const std::string& newState) 
{
	growOlder ();
	
	_previousAngleInRadians = angleInRadians;

	double denormalisedMoveDirection = _previousAngleInRadians - M_PI / 2;
	double newX = x () + speed * cos (denormalisedMoveDirection);
	double newY = y () - speed * sin (denormalisedMoveDirection);

	newX = std::max (-WORLD_SIZE ().x (), std::min (WORLD_SIZE ().x (), newX));
	newY = std::max (-WORLD_SIZE ().y (), std::min (WORLD_SIZE ().y (), newY));
	
	_points.push_back (Pt<double> (newX, newY));
	while (_points.size () > 200)
		_points.erase (_points.begin (), _points.begin () + 1);

	_state = newState;

	if (speed > _speed)
	{
		if (_fatigue < _endurance) _fatigue++;
	}
	else
	{
		if (_fatigue > 0) _fatigue--;
	}
	if (_fatigue == 0) _tired = false;
	if (_fatigue == _endurance) _tired = true; 

	getHungrier (speed);
}
       
void Blob::eat (Food* food, const std::string& state)
{
	_state = state;
	_hunger -= food->takeABite (_hunger);
}
 
unsigned int Blob::takeABite (unsigned int biteSize)
{
	unsigned int biteTaken = std::min (biteSize, _size);
	_size -= biteTaken;
	return biteTaken;
}
 
std::shared_ptr <Action> Blob::createActionDead ()
{
	return std::shared_ptr<Action> (new Movement (this, "dead", 0, 0));
}
 
std::shared_ptr <Action> Blob::createActionWander ()
{
	double angle = _moveDirectionFn (_previousAngleInRadians);
	return std::shared_ptr<Action> (new Movement (this, "wandering", _speed, angle));
}
        
std::shared_ptr <Action> Blob::createActionFlee (const Blob& target)
{
	return std::shared_ptr <Action> (new Movement (this,
			 "running from " + target.name () + (!isTired () ? " (fast)" : ""),
			isTired () ? _speed : _runningSpeed,
			_moveDirectionFn ((0.9 * _previousAngleInRadians + 0.1 * (angle (target) + M_PI)))));
}
       
std::shared_ptr <Action> Blob::createActionHunt (const Blob& target)
{
	return std::shared_ptr <Action> (new Movement (this,
		 "hunting " + target.name () + (!isTired () ? " (fast)" : ""),
		std::min (isTired() ? _speed : _runningSpeed, distance (target)),
		angle (target)));
}  

std::shared_ptr <Action> Blob::createActionFight (Blob& target)
{
	return std::shared_ptr <Action> (new Fight (&target, this, "fighting " + target.name ()));
}

std::shared_ptr <Action> Blob::createActionAttack (Blob& target)
{
	if (isInSameSquare (target))
	{
		return createActionFight (target);
	}
	else
	{
		return createActionHunt (target);
	}
}

std::shared_ptr <Action> Blob::createActionEat (Blob& food)
{
	return std::shared_ptr <Action> (new Eat (this, &food, " eating" + food.name ()));
}

double Blob::relativeDifference (double v1, double v2)
{
	assert (v1 >= 0.0);
	assert (v2 >= 0.0);
	return (v1 - v2) / (v1 + v2) / 2; 
}

double Blob::inflictDamageWeight (const Blob& b) const
{
	return relativeDifference (damage (), b.HP ()) * 2.0;
}

double Blob::hungerWeight (const Blob& b) const
{
	return std::min (1.0, ((double) b.size ()) / 2000.0) * (1.0 - hungerRatio ());
}

double Blob::avoidDamageWeight (const Blob& b) const
{
	return relativeDifference (b.damage () + 1, HP ()) * 2.0;
}

double Blob::distanceWeight (const Blob& b) const
{
	if (isInSameSquare (b))
	{
		return 1.0;
	}
	else if (canSmell (b)) 
	{
		return 1.0 - (distance (b) / smell ());
	}
	else
	{
		return 0.0;
	}
}

double Blob::attackWeight (const Blob& b) const
{
	return distanceWeight (b) * inflictDamageWeight (b);
}

double Blob::fleeWeight (const Blob& b) const
{
	return distanceWeight (b) * avoidDamageWeight (b);
}

std::vector<Option> Blob::findOptions (std::vector<Blob>& others) const
{	
	std::vector <Option> options;

	for (auto& b : others)
	{
		if (&b != this)
		{
			if (b.isDead ())
			{
 				if (b.size () > 0U && hungerRatio () < 0.30)
				{
					if (isInSameSquare (b))
					{
						options.push_back (Option (::eat, 2 * hungerWeight (b) + _aggressionFn (_aggression), &b));
					}
					else if (canSmell (b))
					{
						options.push_back (Option (attack, (2 * hungerWeight (b) * distanceWeight (b)) + _aggressionFn (_aggression), &b));
					}
				}
			}
			else if (isInSameSquare (b) || canSmell (b))
			{
				options.push_back (Option (attack, attackWeight (b) + _aggressionFn (_aggression), &b));
				options.push_back (Option {flee, fleeWeight (b) - _aggressionFn (_aggression), &b});
			}
		}
	}
	
	return options;
}

std::vector<Option> Blob::prioritiseOptions (const std::vector <Option>& options)
{
	std::vector<Option> result = options;

	std::sort (result.begin (), result.end (),
		   [] (const Option& lhs,
		       const Option& rhs) {
		return lhs.weight () < rhs.weight ();});
		
	return result;
}

Option Blob::selectBestOption (const std::vector <Option>& options)
{
	if (options.size () > 0)
	{	
		std::vector<Option>  prioritisedOptions = prioritiseOptions (options);
	
		return prioritisedOptions.back ();
	}
	return Option (wander, 0.0, 0);
}

Option Blob::chooseBestOption (std::vector<Blob>& blobs)
{
	return selectBestOption (findOptions (blobs));
}

std::shared_ptr <Action> Blob::chooseNextAction (std::vector<Blob>& blobs)
{
	if (isDead())
	{
		return createActionDead ();
	}
	else
	{
		auto selectedOption = chooseBestOption (blobs);
		switch (selectedOption.action ())
		{
			case attack:
 					return createActionAttack (*(selectedOption.target ()));
			case flee:
				return createActionFlee (*(selectedOption.target ()));
			case wander:
				return createActionWander ();
			case ::eat:
				return createActionEat (*(selectedOption.target ()));
		}
	}
}

Pt<double> Blob::WORLD_SIZE ()
{
	static Pt<double> r (1500.0, 750.0);
	return r;
}

std::ostream& operator<< (std::ostream& s, const Blob& b)
{
	s << b.x () << "," << b.y ();
	return s; 
}
