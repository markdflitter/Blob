#include "blobImpl.h"

BlobImpl::BlobImpl (const CreateBlob& params) :
	  _name (params._name)
	, _wanderingSpeed (params._speed)
	, _runningSpeed (params._runningSpeed)
	, _baseSmell (params._smell)
	, _baseHP (params._HP)
	, _endurance (params._endurance)
	, _aggression (params._aggression)
	, _lifespan (params._lifespan)
	, _baseDamage (params._damage)
	, _starvationLevel (params._maxHunger)
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
	setHP (maxHP ());
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

unsigned int BlobImpl::maxHP () const
{
	double scalingFactor = propertyScalingFactorDueToAge () * propertyScalingFactorDueToHunger ();
	return ((unsigned int) ((_baseHP * scalingFactor) + 0.5));
}

void BlobImpl::setHP (unsigned int newHP)
{
	_HP = newHP;
	if (_HP == 0U)
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
	_baseSmell = 0.0;
	_baseHP = 0U;
	_HP = 0U;
	_baseDamage = 0U;
	_endurance = 0U;
	_aggression = 0.0;
	_starvationLevel = 0U;
	_currentHunger = 0.0;
}



