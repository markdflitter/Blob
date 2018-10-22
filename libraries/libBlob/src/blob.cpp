#include "blob.h"

#include "blobImpl.h"
#include "rnd.h"

#include <algorithm>
#include <sstream>
#include <math.h>
#include <assert.h>

Blob::Blob (const CreateBlob& params) :
	_impl (std::make_shared <BlobImpl> (params))
{
}

Blob::Blob (const Blob& other) :
	_impl (std::make_shared <BlobImpl> (*(other._impl)))
{
}

Pt<double> Blob::WORLD_SIZE ()
{
	static Pt<double> r (1500.0, 750.0);
	return r;
}

std::shared_ptr <BlobImpl> Blob::getImpl ()
{
	return _impl;
}

std::string Blob::name () const
{
	return _impl->_name;
}

unsigned int Blob::lifespan () const
{
	return _impl->_lifespan;
}

unsigned int Blob::currentAge () const
{
	return _impl->_currentAge;
}

unsigned int Blob::starvationLevel () const
{
	return _impl->_starvationLevel;
}

double Blob::currentHunger () const
{
	return _impl->_currentHunger;
}

unsigned int Blob::greatestMaxHP () const
{
	return _impl->_greatestMaxHP;
}

unsigned int Blob::currentMaxHP () const
{
	return _impl->currentMaxHP ();
}

unsigned int Blob::currentHP () const
{
	return _impl->_currentHP;
}

double Blob::greatestWanderingSpeed () const
{
	return _impl->_wanderingSpeed;
}

double Blob::greatestRunningSpeed () const
{
	return _impl->_runningSpeed;
}

double Blob::currentWanderingSpeed () const
{
	return _impl->scalePropertyByHPRatio (_impl->_wanderingSpeed);
}

double Blob::currentRunningSpeed () const
{
	return _impl->scalePropertyByHPRatio (_impl->_runningSpeed);
}
	
double Blob::currentSmell () const
{
	return _impl->_smell * _impl->propertyScalingFactorDueToAge ();
}

unsigned int Blob::greatestDamage () const
{
	return _impl->_damage;
}

unsigned int Blob::currentDamage () const
{
	return static_cast <unsigned int> (_impl->scalePropertyByHPRatio (_impl->_damage) + 0.5);
}

unsigned int Blob::endurance () const
{
	return _impl->_endurance;
}

double Blob::currentFatigue () const
{
	return _impl->_fatigue;
}

unsigned int Blob::currentSize () const
{
	return ((unsigned int) ((((double) _impl->_size) * _impl->propertyScalingFactorDueToAge ()) + 0.5)) ;
}

double Blob::currentAggression () const
{
	return isDead () ? _impl->_aggression : ((2.0 - _impl->propertyScalingFactorDueToHunger ()) * _impl->_aggression);
}

const std::vector<Pt<double>>& Blob::history () const
{
	return _impl->_points;
}

std::string Blob::state () const
{
	return _impl->_state;
}

bool Blob::isTired () const
{
	return _impl->_tired;
}

bool Blob::isDead () const
{
	return _impl->_dead;
}

double Blob::distance (const Blob& other) const
{
	Pt<double> p1 = other.history ().back ();
	Pt<double> p2 = history ().back ();

	double dx = p1.x () - p2.x ();
	double dy = p1.y () - p2.y ();
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
	return isInRange (other, currentSmell ());
}

double Blob::angle (const Blob& other) const
{
	Pt<double> p1 = history ().back ();
	Pt<double> p2 = other.history ().back ();

	double dx = p1.x () - p2.x ();
	double dy = p1.y () - p2.y ();

	return atan2 (dy, -dx) + M_PI/2;
}

void Blob::limitHPtoMax (unsigned int previousDamage)
{
	// this is so that if we increase max (due to aging), then our current HP go up by the same amount
	_impl->setHP (_impl->currentMaxHP () - previousDamage);
}

void Blob::growOlder ()
{
	if (!isDead ())
	{
		unsigned int previousDamage = _impl->currentMaxHP () - _impl->_currentHP;
		if (++_impl->_currentAge >= lifespan ())
		{
			_impl->kill ();
		}	
		limitHPtoMax (previousDamage);
		if (_impl->_currentHP < _impl->currentMaxHP ())
		{
			_impl->setHP (_impl->_currentHP + 1U);
		}
	}
}

void Blob::getHungrier (double amount)
{
	unsigned int previousDamage = _impl->currentMaxHP () - _impl->_currentHP;
	_impl->_currentHunger += amount;
	if (_impl->_currentHunger > (double) _impl->_starvationLevel)
	{
		_impl->_currentHunger = (double) _impl->_starvationLevel;
	}
	limitHPtoMax (previousDamage);
}

void Blob::takeDamage (unsigned int damage)
{
	if (_impl->_currentHP >= damage)
	{
		_impl->setHP (_impl->_currentHP - damage);
	}
	else
	{
		_impl->setHP (0);
	}
}

void Blob::inflictDamage (Target* target, const std::string& state)
{
	_impl->_state = state;
	growOlder ();
	target->takeDamage (currentDamage ());
	getHungrier (currentDamage ());
}

void Blob::retaliate (Target* target)
{
	target->takeDamage (currentDamage ());
}

void Blob::move (double speed, double angleInRadians, const std::string& newState) 
{
	growOlder ();
	
	_impl->_previousAngleInRadians = angleInRadians;

	Pt<double> p = history ().back ();

	double denormalisedMoveDirection = _impl->_previousAngleInRadians - M_PI / 2;
	double newX = p.x () + speed * cos (denormalisedMoveDirection);
	double newY = p.y () - speed * sin (denormalisedMoveDirection);

	newX = std::max (-WORLD_SIZE ().x (), std::min (WORLD_SIZE ().x (), newX));
	newY = std::max (-WORLD_SIZE ().y (), std::min (WORLD_SIZE ().y (), newY));
	
	_impl->_points.push_back (Pt<double> (newX, newY));
	while (_impl->_points.size () > 200)
		_impl->_points.erase (_impl->_points.begin (), _impl->_points.begin () + 1);

	_impl->_state = newState;

	if (speed > currentWanderingSpeed ())
	{
		if (_impl->_fatigue < _impl->_endurance) _impl->_fatigue++;
	}
	else
	{
		if (_impl->_fatigue > 0) _impl->_fatigue--;
	}
	if (_impl->_fatigue == 0) _impl->_tired = false;
	if (_impl->_fatigue == _impl->_endurance) _impl->_tired = true; 

	getHungrier (speed);
}
       
void Blob::eat (Food* food, const std::string& state)
{
	_impl->_state = state;
	_impl->_currentHunger -= food->takeABite (_impl->_currentHunger);
}
 
unsigned int Blob::takeABite (unsigned int biteSize)
{
	unsigned int biteTaken = std::min (biteSize, _impl->_size);
	_impl->_size -= biteTaken;
	return biteTaken;
}
 
std::shared_ptr <Action> Blob::createActionDead ()
{
	return std::shared_ptr<Action> (new Movement (this, "dead", 0, 0));
}
 
std::shared_ptr <Action> Blob::createActionWander ()
{
	double angle = _impl->_moveDirectionFn (_impl->_previousAngleInRadians);
	return std::shared_ptr<Action> (new Movement (this, "wandering", currentWanderingSpeed (), angle));
}
        
std::shared_ptr <Action> Blob::createActionFlee (const Blob& target)
{
	return std::shared_ptr <Action> (new Movement (this,
			 "running from " + target.name () + (!_impl->_tired ? " (fast)" : ""),
			_impl->_tired ? currentWanderingSpeed () : currentRunningSpeed (),
			_impl->_moveDirectionFn ((0.9 * _impl->_previousAngleInRadians + 0.1 * (angle (target) + M_PI)))));
}
       
std::shared_ptr <Action> Blob::createActionHunt (const Blob& target)
{
	return std::shared_ptr <Action> (new Movement (this,
		 "hunting " + target.name () + (!_impl->_tired ? " (fast)" : ""),
		std::min (_impl->_tired ? currentWanderingSpeed () : currentRunningSpeed (), distance (target)),
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
	return relativeDifference (currentDamage (), b._impl->_currentHP) * 2.0;
}

double Blob::hungerWeight (const Blob& b) const
{
	return std::min (1.0, ((double) b.currentSize ()) / 2000.0) * (1.0 - _impl->propertyScalingFactorDueToHunger ());
}

double Blob::avoidDamageWeight (const Blob& b) const
{
	return relativeDifference (b.currentDamage () + 1, _impl->_currentHP) * 2.0;
}

double Blob::distanceWeight (const Blob& b) const
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
 				if (b.currentSize () > 0U && _impl->propertyScalingFactorDueToHunger () < 0.30)
				{
					if (isInSameSquare (b))
					{
						options.push_back (Option (::eat, 2 * hungerWeight (b) + _impl->_aggressionFn (_impl->_aggression), &b));
					}
					else if (canSmell (b))
					{
						options.push_back (Option (attack, (2 * hungerWeight (b) * distanceWeight (b)) + _impl->_aggressionFn (_impl->_aggression), &b));
					}
				}
			}
			else if (isInSameSquare (b) || canSmell (b))
			{
				options.push_back (Option (attack, attackWeight (b) + _impl->_aggressionFn (_impl->_aggression), &b));
				options.push_back (Option {flee, fleeWeight (b) - _impl->_aggressionFn (_impl->_aggression), &b});
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

void Blob::kill ()
{
	_impl->kill ();
}

double Blob::fade () const
{
	double timeLeft = (((double) lifespan ()) - currentAge ()) / lifespan () * _impl->propertyScalingFactorDueToHunger ();
	return isDead () ? 0.4 : ((0.6 * timeLeft) + 0.4);
}

	
