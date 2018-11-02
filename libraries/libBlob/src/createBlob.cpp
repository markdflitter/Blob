#include "createBlob.h"

CreateBlob::CreateBlob () :
        _name ("")
      , _position (make_pt (0.0, 0.0))
      , _speed (0.0)
      , _runningSpeed (0.0)
      , _smell (0.0)
      , _HP (0U)
      , _endurance (0U)
      , _aggression (0.0)
      , _lifespan (0U)
      , _damage (0U)
      , _maxHunger (0U)
      , _size (0U)
      , _moveDirectionFn ([] (double) {return 0.0;})
      , _aggressionFn ([](double a) {return a;})
{
}

CreateBlob CreateBlob::name (const std::string& name)
{
	CreateBlob b (*this);
	b._name = name;
	return b;
}

CreateBlob CreateBlob::position (const Pt<double>& position)
{
	CreateBlob b (*this);
	b._position = position;
	return b;
}

CreateBlob CreateBlob::speed (double speed)
{
	CreateBlob b (*this);
	b._speed = speed;
	return b;
}

CreateBlob CreateBlob::runningSpeed (double runningSpeed)
{
	CreateBlob b (*this);
	b._runningSpeed = runningSpeed;
	return b;
}

CreateBlob CreateBlob::smell (double smell)
{
	CreateBlob b (*this);
	b._smell = smell;
	return b;
}

CreateBlob CreateBlob::HP (unsigned int HP)
{
	CreateBlob b (*this);
	b._HP = HP;
	return b;
}

CreateBlob CreateBlob::endurance (unsigned int endurance)
{
	CreateBlob b (*this);
	b._endurance = endurance;
	return b;
}

CreateBlob CreateBlob::aggression (double aggression)
{
	CreateBlob b (*this);
	b._aggression = aggression;
	return b;
}

CreateBlob CreateBlob::lifespan (unsigned int lifespan)
{
	CreateBlob b (*this);
	b._lifespan = lifespan;
	return b;
}

CreateBlob CreateBlob::damage (unsigned int damage)
{
	CreateBlob b (*this);
	b._damage = damage;
	return b;
}

CreateBlob CreateBlob::maxHunger (unsigned int maxHunger)
{
	CreateBlob b (*this);
	b._maxHunger = maxHunger;
	return b;
}

CreateBlob CreateBlob::size (unsigned int size)
{
	CreateBlob b (*this);
	b._size = size;
	return b;
}

CreateBlob CreateBlob::moveDirectionFn (std::function<double(double)> moveDirectionFn)
{
	CreateBlob b (*this);
	b._moveDirectionFn = moveDirectionFn;
	return b;
}

CreateBlob CreateBlob::aggressionFn (std::function<double(double)> aggressionFn)
{
	CreateBlob b (*this);
	b._aggressionFn = aggressionFn;
	return b;
}

