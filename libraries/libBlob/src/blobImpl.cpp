#include "blobImpl.h"

BlobImpl::BlobImpl (const CreateBlob& params) :
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

double BlobImpl::wanderingSpeed () const
{
	return (_baseHP == 0) ? 0U : _speed * (double (_HP)) / _baseHP;
}

double BlobImpl::runningSpeed () const
{
	return (_baseHP == 0) ? 0U : _runningSpeed * (double (_HP)) / _baseHP;
}

Pt<double> BlobImpl::position () const
{
	return _points.back ();
}

double BlobImpl::ageRatio () const
{
	double a = -2.0 / (((double) _lifespan) * _lifespan);
	double b = 2.0 / ((double) _lifespan);
	double c = 0.5;

	return a * ((double) _age) * _age + b * ((double) _age) + c;
}

double BlobImpl::hungerRatio () const
{
	return (_maxHunger - _hunger) / _maxHunger;
}

unsigned int BlobImpl::maxHP () const
{
	return ((unsigned int) ((_baseHP * hungerRatio () * ageRatio ()) + 0.5));
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

