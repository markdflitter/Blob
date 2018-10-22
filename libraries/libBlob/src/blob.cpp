#include "blob.h"

#include "blobImpl.h"
#include "rnd.h"

#include <algorithm>
#include <sstream>
#include <math.h>

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
	return BlobImpl::WORLD_SIZE ();
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
	return _impl->currentWanderingSpeed ();
}

double Blob::currentRunningSpeed () const
{
	return _impl->currentRunningSpeed ();
}
	
double Blob::currentSmell () const
{
	return _impl->currentSmell ();
}

unsigned int Blob::greatestDamage () const
{
	return _impl->_damage;
}

unsigned int Blob::currentDamage () const
{
	return _impl->currentDamage ();
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
	return 	_impl->distance (other._impl);
}

bool Blob::isInRange (const Blob& other, double range) const
{
	return 	_impl->isInRange (other._impl, range);
}

bool Blob::isInSameSquare (const Blob& other) const
{
	return isInSameSquare (other._impl);
}

bool Blob::canSmell (const Blob& other) const
{
	return _impl->canSmell (other._impl);
}

double Blob::angle (const Blob& other) const
{
	return _impl->angle (other._impl);
}

double Blob::alpha () const
{
	double timeLeft = (((double) lifespan ()) - currentAge ()) / lifespan () * _impl->propertyScalingFactorDueToHunger ();
	return isDead () ? 0.4 : ((0.6 * timeLeft) + 0.4);
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


