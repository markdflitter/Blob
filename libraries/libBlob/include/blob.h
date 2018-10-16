#ifndef INCLUDED_BLOB_H
#define INCLUDED_BLOB_H

#include <functional>
#include <algorithm>
#include <iostream>
#include <math.h>
#include <string>
#include <vector>
#include <sstream>
#include <memory>
#include <pt.h>
#include <action.h>
#include <movement.h>
#include <fight.h>
#include <eat.h>
#include <rnd.h>
#include "option.h"
#include <assert.h>
	
class CreateBlob 
{
public:
	CreateBlob () :
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

	CreateBlob name (const std::string& name) {CreateBlob b (*this); b._name = name; return b;}
	CreateBlob position (const Pt<double>& position) {CreateBlob b (*this); b._position = position; return b;}
	CreateBlob speed (double speed) {CreateBlob b (*this); b._speed = speed; return b;}
	CreateBlob runningSpeed (double runningSpeed) {CreateBlob b (*this); b._runningSpeed = runningSpeed; return b;}
	CreateBlob smell (double smell) {CreateBlob b (*this); b._smell = smell; return b;}
	CreateBlob HP (unsigned int HP) {CreateBlob b (*this); b._HP = HP; return b;}
	CreateBlob endurance (unsigned int endurance) {CreateBlob b (*this); b._endurance = endurance; return b;}
	CreateBlob aggression (double aggression) {CreateBlob b (*this); b._aggression = aggression; return b;}
	CreateBlob lifespan (unsigned int lifespan) {CreateBlob b (*this); b._lifespan = lifespan; return b;}
	CreateBlob damage (unsigned int damage) {CreateBlob b (*this); b._damage = damage; return b;}
	CreateBlob maxHunger (unsigned int maxHunger) {CreateBlob b (*this); b._maxHunger = maxHunger; return b;}
	CreateBlob size (unsigned int size) {CreateBlob b (*this); b._size = size; return b;}
	CreateBlob moveDirectionFn (std::function<double(double)> moveDirectionFn)
		{CreateBlob b (*this); b._moveDirectionFn = moveDirectionFn; return b;}
	CreateBlob aggressionFn (std::function<double(double)> aggressionFn)
		{CreateBlob b (*this); b._aggressionFn = aggressionFn; return b;}
private:
	friend class Blob;
	
	std::string _name;
	Pt<double> _position;
	double _speed;
        double _runningSpeed;
	double _smell;
	unsigned int _HP;
	unsigned int _endurance;
	double _aggression;
	unsigned int _lifespan;
	unsigned int _damage;
	unsigned int _maxHunger;
	unsigned int _size;

	std::function<double(double)> _moveDirectionFn;
	std::function<double(double)> _aggressionFn;
};

class Blob : public Moveable, public Target, public Eater, public Food
{
public:
	Blob (const CreateBlob& params) :
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

	std::string name () const {return _name;}
	double x () const {return _points.back ().x ();}
	double y () const {return _points.back ().y ();}
	double baseSpeed () const {return _speed;}
	double speed () const {return (baseHP () == 0) ? 0U : _speed * (double (_HP)) / baseHP ();}
	double runningSpeed () const {return (baseHP () == 0) ? 0U :_runningSpeed * (double (_HP)) / baseHP ();}
	double smell () const {return _baseSmell * ageRatio ();}
	unsigned int baseHP () const {return _baseHP;}
	unsigned int HP () const {return _HP;}
	unsigned int maxHP () const {return ((unsigned int) ((_baseHP * hungerRatio () * ageRatio ()) + 0.5));}
	unsigned int baseDamage () const {return _baseDamage;}
	unsigned int damage () const {return (baseHP () == 0) ? 0U : ((unsigned int) ((_baseDamage * (double (_HP)) / baseHP ()) + 0.5));}
	unsigned int endurance () const {return _endurance;}
	double aggression () const {return isDead () ? _aggression : ((2.0 - hungerRatio ()) * _aggression);}
 	unsigned int maxHunger () const {return _maxHunger;}
 	double hunger () const {return _hunger;}
 	unsigned int size () const {return ((unsigned int) ((((double) _size) * ageRatio ()) + 0.5)) ;}
	unsigned int lifespan () const {return _lifespan;}
	unsigned int age () const {return _age;}
	double ageRatio () const
	{
		double a = -2.0 / (((double) _lifespan) * _lifespan);
		double b = 2.0 / ((double) _lifespan);
		double c = 0.5;

		return a * ((double) _age) * _age + b * ((double) _age) + c;
	}
	double hungerRatio () const {return (_maxHunger - _hunger) / _maxHunger;}
	std::string state () const {return _state;}

	double fatigue () const {return _fatigue;}
	bool isTired () const {return _tired;} 

	bool isDead () const {return _dead;}

	friend std::ostream& operator<<(std::ostream& s, const Blob& b);

	const std::vector<Pt<double>>& history () const {return _points;}

	double distance (const Blob& other) const
	{
		double dx = other.x () - x();
		double dy = other.y () - y();
		double d = sqrt ((dx * dx) + (dy * dy));

		return d; 
	}

	bool isInRange (const Blob& other, double range) const
	{
		return distance (other) <= range;
	}

	bool isInSameSquare (const Blob& other) const
	{
		return isInRange (other, sqrt (2));
	}
	
	bool canSmell (const Blob& other) const
	{
		return isInRange (other, smell ());
	}

	double angle (const Blob& other) const
	{
		double dx = x () - other.x ();
		double dy = y () - other.y ();

		return atan2 (dy, -dx) + M_PI/2;
	}

        void kill ()
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

	void setHP (unsigned int newHP)
	{
		_HP = newHP;
		if (_HP == 0U)
		{
			kill ();
		}
	}

	void limitHPtoMax (unsigned int previousDamage)
	{
		// this is so that if we increase max (due to aging), then our current HP go up by the same amount
		setHP (maxHP () - previousDamage);
	}

	void growOlder ()
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

	void getHungrier (double amount)
	{
		unsigned int previousDamage = maxHP () - HP ();
		_hunger += amount;
		if (_hunger > (double) _maxHunger)
		{
			_hunger = (double) _maxHunger;
		}
		limitHPtoMax (previousDamage);
	}

	void takeDamage (unsigned int damage)
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

	void inflictDamage (Target* target, const std::string& state)
	{
		_state = _state;
		growOlder ();
		target->takeDamage (damage ());
		getHungrier (damage ());
	}

	void retaliate (Target* target)
	{
		target->takeDamage (damage ());
	}

        void move (double speed, double angleInRadians, const std::string& newState) 
	{
		growOlder ();
		
		_previousAngleInRadians = angleInRadians;

		double denormalisedMoveDirection = _previousAngleInRadians - M_PI / 2;
		double newX = x () + speed * cos (denormalisedMoveDirection);
		double newY = y () - speed * sin (denormalisedMoveDirection);
	
		newX = std::max (-WORLD_SIZE.x (), std::min (WORLD_SIZE.x (), newX));
		newY = std::max (-WORLD_SIZE.y (), std::min (WORLD_SIZE.y (), newY));
		
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
       
	void eat (Food* food, const std::string& state)
	{
		_state = state;
		_hunger -= food->takeABite (_hunger);
	}
 
	unsigned int takeABite (unsigned int biteSize)
	{
		unsigned int biteTaken = std::min (biteSize, _size);
		_size -= biteTaken;
		return biteTaken;
	}
 
	std::shared_ptr <Action> createActionDead ()
        {
		return std::shared_ptr<Action> (new Movement (this, "dead", 0, 0));
	}
 
        std::shared_ptr <Action> createActionWander ()
        {
		double angle = _moveDirectionFn (_previousAngleInRadians);
		return std::shared_ptr<Action> (new Movement (this, "wandering", _speed, angle));
	}
        
	std::shared_ptr <Action> createActionFlee (const Blob& target)
        {
		return std::shared_ptr <Action> (new Movement (this,
				 "running from " + target.name () + (!isTired () ? " (fast)" : ""),
				isTired () ? _speed : _runningSpeed,
				_moveDirectionFn ((0.9 * _previousAngleInRadians + 0.1 * (angle (target) + M_PI)))));
	}
       
	std::shared_ptr <Action> createActionHunt (const Blob& target)
        {
		return std::shared_ptr <Action> (new Movement (this,
			 "hunting " + target.name () + (!isTired () ? " (fast)" : ""),
			std::min (isTired() ? _speed : _runningSpeed, distance (target)),
			angle (target)));
	}  
	
	std::shared_ptr <Action> createActionFight (Blob& target)
        {
		return std::shared_ptr <Action> (new Fight (&target, this, "fighting " + target.name ()));
	}

	std::shared_ptr <Action> createActionAttack (Blob& target)
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

	std::shared_ptr <Action> createActionEat (Blob& food)
	{
		return std::shared_ptr <Action> (new Eat (this, &food, " eating" + food.name ()));
	}

	double relativeDifference (double v1, double v2) const
	{
		assert (v1 >= 0.0);
		assert (v2 >= 0.0);
		return (v1 - v2) / (v1 + v2) / 2; 
	}

	double inflictDamageWeight (const Blob& b) const
	{
		return relativeDifference (damage (), b.HP ()) * 2.0;
	}

	double hungerWeight (const Blob& b) const
	{
		return std::min (1.0, ((double) b.size ()) / 2000.0) * (1.0 - hungerRatio ());
	}


	double avoidDamageWeight (const Blob& b) const
	{
		return relativeDifference (b.damage () + 1, HP ()) * 2.0;
	}

	double distanceWeight (const Blob& b) const
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

	double attackWeight (const Blob& b) const
	{
		return distanceWeight (b) * inflictDamageWeight (b);
	}
	
	double fleeWeight (const Blob& b) const
	{
		return distanceWeight (b) * avoidDamageWeight (b);
	}

	std::vector<Option> findOptions (std::vector<Blob>& others) const
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

	std::vector<Option> prioritiseOptions (const std::vector <Option>& options)
	{
		std::vector<Option> result = options;

		std::sort (result.begin (), result.end (),
			   [] (const Option& lhs,
			       const Option& rhs) {
			return lhs.weight () < rhs.weight ();});
			
		return result;
	}

	Option selectBestOption (const std::vector <Option>& options)
	{
		if (options.size () > 0)
		{	
			std::vector<Option>  prioritisedOptions = prioritiseOptions (options);
		
			return prioritisedOptions.back ();
		}
		return Option (wander, 0.0, 0);
	}

	Option chooseBestOption (std::vector<Blob>& blobs)
	{
		return selectBestOption (findOptions (blobs));
	}

	std::shared_ptr <Action> chooseNextAction (std::vector<Blob>& blobs)
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
private:
	Pt<double> WORLD_SIZE = Pt<double>(1500.0, 750.0);

	std::vector<Pt<double>> _points;

	std::string _name;
	double _speed;
        double _runningSpeed;
	double _baseSmell;
	unsigned int _baseHP;
	unsigned int _HP;
	unsigned int _endurance;
	double _aggression;
	unsigned int _lifespan;
	unsigned int _baseDamage;
	unsigned int _maxHunger;
	double _hunger;
	unsigned int _size;

	double _previousAngleInRadians = 0;

	std::string _state;

	unsigned int _fatigue;
	bool _tired;

	unsigned int _age;	
	bool _dead;

	std::function<double(double)> _moveDirectionFn;
	std::function<double(double)> _aggressionFn;
};

inline std::ostream& operator<< (std::ostream& s, const Blob& b)
{
	s << b.x () << "," << b.y ();
	return s; 
}

#endif

