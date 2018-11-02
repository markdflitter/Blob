#include "blobImpl.h"

BlobImpl::BlobImpl (const CreateBlob& params) :
	  _name (params._name)
	, _lifespan (params._lifespan)
	, _starvationLevel (params._starvationLevel)
	, _greatestWanderingSpeed (params._speed)
	, _greatestRunningSpeed (params._runningSpeed)
	, _smell (params._smell)
	, _greatestMaxHP (params._HP)
	, _endurance (params._endurance)
	, _aggression (params._aggression)
	, _baseDamage (params._damage)
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
	_greatestWanderingSpeed = 0.0;
	_greatestRunningSpeed = 0.0;
	_smell = 0.0;
	_greatestMaxHP = 0U;
	_currentHP = 0U;
	_baseDamage = 0U;
	_endurance = 0U;
	_aggression = 0.0;
	_starvationLevel = 0U;
	_currentHunger = 0.0;
}



