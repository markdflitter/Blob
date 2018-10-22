#include "blobImpl.h"
#include <assert.h>

BlobImpl::BlobImpl (const CreateBlob& params) :
	  _name (params._name)
	, _lifespan (params._lifespan)
	, _starvationLevel (params._starvationLevel)
	, _wanderingSpeed (params._speed)
	, _runningSpeed (params._runningSpeed)
	, _smell (params._smell)
	, _greatestMaxHP (params._HP)
	, _endurance (params._endurance)
	, _aggression (params._aggression)
	, _damage (params._damage)
	, _size (params._size)
	, _moveDirectionFn (params._moveDirectionFn)
	, _aggressionFn (params._aggressionFn)
	,  _state ("newborn")
 	, _fatigue (0)
	, _tired (false)
	, _currentAge (0)
	, _currentHunger (0.0)
	, _dead (false)
{
	_points.push_back (params._position);
	setHP (currentMaxHP ());
}

double BlobImpl::currentWanderingSpeed () const
{
	return scalePropertyByHPRatio (_wanderingSpeed);
}

double BlobImpl::currentRunningSpeed () const
{
	return scalePropertyByHPRatio (_runningSpeed);
}
	
double BlobImpl::currentSmell () const
{
	return _smell * propertyScalingFactorDueToAge ();
}


double BlobImpl::scalePropertyByHPRatio (double property) const
{
	return (_greatestMaxHP == 0U) ? 0U : property * (double (_currentHP)) / _greatestMaxHP;
}

double BlobImpl::propertyScalingFactorDueToAge () const
{
	double a = -2.0 / (((double) _lifespan) * _lifespan);
	double b = 2.0 / ((double) _lifespan);
	double c = 0.5;

	return a * ((double) _currentAge) * _currentAge + b * ((double) _currentAge) + c;
}

double BlobImpl::propertyScalingFactorDueToHunger () const
{
	return (_starvationLevel - _currentHunger) / _starvationLevel;
}

unsigned int BlobImpl::currentMaxHP () const
{
	double scalingFactor = propertyScalingFactorDueToAge () * propertyScalingFactorDueToHunger ();
	return ((unsigned int) ((_greatestMaxHP * scalingFactor) + 0.5));
}

void BlobImpl::setHP (unsigned int newHP)
{
	_currentHP = newHP;
	if (_currentHP == 0U)
	{
		kill ();
	}
}

void BlobImpl::kill ()
{
	_dead = true;
	_state = "dead";
	_wanderingSpeed = 0.0;
	_runningSpeed = 0.0;
	_smell = 0.0;
	_greatestMaxHP = 0U;
	_currentHP = 0U;
	_damage = 0U;
	_endurance = 0U;
	_aggression = 0.0;
	_starvationLevel = 0U;
	_currentHunger = 0.0;
}
	
void BlobImpl::limitHPtoMax (unsigned int previousDamage)
{
	// this is so that if we increase max (due to aging), then our current HP go up by the same amount
	setHP (currentMaxHP () - previousDamage);
}

void BlobImpl::growOlder ()
{
	if (!_dead)
	{
		unsigned int previousDamage = currentMaxHP () - _currentHP;
		if (++_currentAge >= _lifespan)
		{
			kill ();
		}	
		limitHPtoMax (previousDamage);
		if (_currentHP < currentMaxHP ())
		{
			setHP (_currentHP + 1U);
		}
	}
}

void BlobImpl::getHungrier (double amount)
{
	unsigned int previousDamage = currentMaxHP () - _currentHP;
	_currentHunger += amount;
	if (_currentHunger > (double) _starvationLevel)
	{
		_currentHunger = (double) _starvationLevel;
	}
	limitHPtoMax (previousDamage);
}
 
std::shared_ptr <Action> BlobImpl::createActionDead ()
{
	return std::shared_ptr<Action> (new Movement (this, "dead", 0, 0));
}
 
std::shared_ptr <Action> BlobImpl::createActionWander ()
{
	double angle = _moveDirectionFn (_previousAngleInRadians);
	return std::shared_ptr<Action> (new Movement (this, "wandering", currentWanderingSpeed (), angle));
}
        
std::shared_ptr <Action> BlobImpl::createActionFlee (BlobImpl* target)
{
	return std::shared_ptr <Action> (new Movement (this,
			 "running from " + target->_name + (!_tired ? " (fast)" : ""),
			_tired ? currentWanderingSpeed () : currentRunningSpeed (),
			_moveDirectionFn ((0.9 * _previousAngleInRadians + 0.1 * (angle (target) + M_PI)))));
}
       
std::shared_ptr <Action> BlobImpl::createActionHunt (BlobImpl* target)
{
	return std::shared_ptr <Action> (new Movement (this,
		 "hunting " + target->_name + (!_tired ? " (fast)" : ""),
		std::min (_tired ? currentWanderingSpeed () : currentRunningSpeed (), distance (target)),
		angle (target)));
}  


Pt<double> BlobImpl::WORLD_SIZE ()
{
	static Pt<double> r (1500.0, 750.0);
	return r;
}

std::shared_ptr <Action> BlobImpl::createActionFight (BlobImpl* target)
{
	return std::shared_ptr <Action> (new Fight (target, this, "fighting " + target->_name));
}

std::shared_ptr <Action> BlobImpl::createActionAttack (BlobImpl* target)
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

unsigned int BlobImpl::currentSize () const
{
	return (double) _size * propertyScalingFactorDueToAge ();
}

std::shared_ptr <Action> BlobImpl::createActionEat (BlobImpl* food)
{
	return std::shared_ptr <Action> (new Eat (this, food, " eating" + food->_name));
}

double BlobImpl::relativeDifference (double v1, double v2)
{
	assert (v1 >= 0.0);
	assert (v2 >= 0.0);
	return (v1 - v2) / (v1 + v2) / 2; 
}

double BlobImpl::inflictDamageWeight (BlobImpl* b) const
{
	return relativeDifference (currentDamage (), b->_currentHP) * 2.0;
}

double BlobImpl::hungerWeight (BlobImpl* b) const
{
	return std::min (1.0, ((double) b->currentSize ()) / 2000.0) * (1.0 - propertyScalingFactorDueToHunger ());
}

double BlobImpl::avoidDamageWeight (BlobImpl* b) const
{
	return relativeDifference (b->currentDamage () + 1, _currentHP) * 2.0;
}

double BlobImpl::distanceWeight (BlobImpl* b) const
{
	if (isInSameSquare (b))
	{
		return 1.0;
	}
	else if (canSmell (b)) 
	{
		return 1.0 - (distance (b) / currentSmell ());
	}
	else
	{
		return 0.0;
	}
}

double BlobImpl::attackWeight (BlobImpl* b) const
{
	return distanceWeight (b) * inflictDamageWeight (b);
}

double BlobImpl::fleeWeight (BlobImpl* b) const
{
	return distanceWeight (b) * avoidDamageWeight (b);
}

std::vector<Option> BlobImpl::findOptions (std::vector<BlobImpl*> others) const
{	
	std::vector <Option> options;

	for (auto& b : others)
	{
		if (b != this)
		{
			if (b->_dead)
			{
 				if (b->currentSize () > 0U && propertyScalingFactorDueToHunger () < 0.30)
				{
					if (isInSameSquare (b))
					{
						options.push_back (Option (::eat, 2 * hungerWeight (b) + _aggressionFn (_aggression), b));
					}
					else if (canSmell (b))
					{
						options.push_back (Option (attack, (2 * hungerWeight (b) * distanceWeight (b)) + _aggressionFn (_aggression), b));
					}
				}
			}
			else if (isInSameSquare (b) || canSmell (b))
			{
				options.push_back (Option (attack, attackWeight (b) + _aggressionFn (_aggression), b));
				options.push_back (Option {flee, fleeWeight (b) - _aggressionFn (_aggression), b});
			}
		}
	}
	
	return options;
}

std::vector<Option> BlobImpl::prioritiseOptions (const std::vector <Option>& options)
{
	std::vector<Option> result = options;

	std::sort (result.begin (), result.end (),
		   [] (const Option& lhs,
		       const Option& rhs) {
		return lhs.weight () < rhs.weight ();});
		
	return result;
}

Option BlobImpl::selectBestOption (const std::vector <Option>& options)
{
	if (options.size () > 0)
	{	
		std::vector<Option>  prioritisedOptions = prioritiseOptions (options);
	
		return prioritisedOptions.back ();
	}
	return Option (wander, 0.0, 0);
}

Option BlobImpl::chooseBestOption (std::vector<BlobImpl*> blobs)
{
	return selectBestOption (findOptions (blobs));
}

void BlobImpl::takeDamage (unsigned int damage)
{
	if (_currentHP >= damage)
	{
		setHP (_currentHP - damage);
	}
	else
	{
		setHP (0);
	}
}

void BlobImpl::inflictDamage (Target* target, const std::string& state)
{
	_state = state;
	growOlder ();
	target->takeDamage (currentDamage ());
	getHungrier (currentDamage ());
}

void BlobImpl::retaliate (Target* target)
{
	target->takeDamage (currentDamage ());
}

unsigned int BlobImpl::currentDamage () const
{
	return static_cast <unsigned int> (scalePropertyByHPRatio (_damage) + 0.5);
}

void BlobImpl::move (double speed, double angleInRadians, const std::string& newState) 
{
	growOlder ();
	
	_previousAngleInRadians = angleInRadians;

	Pt<double> p = _points.back ();

	double denormalisedMoveDirection = _previousAngleInRadians - M_PI / 2;
	double newX = p.x () + speed * cos (denormalisedMoveDirection);
	double newY = p.y () - speed * sin (denormalisedMoveDirection);

	newX = std::max (-WORLD_SIZE ().x (), std::min (WORLD_SIZE ().x (), newX));
	newY = std::max (-WORLD_SIZE ().y (), std::min (WORLD_SIZE ().y (), newY));
	
	_points.push_back (Pt<double> (newX, newY));
	while (_points.size () > 200)
		_points.erase (_points.begin (), _points.begin () + 1);

	_state = newState;

	if (speed > currentWanderingSpeed ())
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
       
void BlobImpl::eat (Food* food, const std::string& state)
{
	_state = state;
	_currentHunger -= food->takeABite (_currentHunger);
}
 
unsigned int BlobImpl::takeABite (unsigned int biteSize)
{
	unsigned int biteTaken = std::min (biteSize, _size);
	_size -= biteTaken;
	return biteTaken;
}

double BlobImpl::distance (BlobImpl* other) const
{
	Pt<double> p1 = other->_points.back ();
	Pt<double> p2 = _points.back ();

	double dx = p1.x () - p2.x ();
	double dy = p1.y () - p2.y ();
	double d = sqrt ((dx * dx) + (dy * dy));

	return d; 
}

bool BlobImpl::isInRange (BlobImpl* other, double range) const
{
	return distance (other) <= range;
}

bool BlobImpl::isInSameSquare (BlobImpl* other) const
{
	return isInRange (other, sqrt (2));
}

bool BlobImpl::canSmell (BlobImpl* other) const
{
	return isInRange (other, currentSmell ());
}

double BlobImpl::angle (BlobImpl* other) const
{
	Pt<double> p1 = _points.back ();
	Pt<double> p2 = other->_points.back ();

	double dx = p1.x () - p2.x ();
	double dy = p1.y () - p2.y ();

	return atan2 (dy, -dx) + M_PI/2;
}

std::shared_ptr <Action> BlobImpl::chooseNextAction (std::vector<BlobImpl*> blobs)
{
	std::vector<BlobImpl*> v;
	for (auto b : blobs
	return _impl->chooseNextAction (v);
}
