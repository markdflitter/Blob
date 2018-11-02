#ifndef INCLUDED_OPTION_H
#define INCLUDED_OPTION_H

enum action_t {attack, flee, wander, eat};

class Blob;

class Option 
{
public:
	Option (action_t action, double weight, Blob* target) :
		_action (action)
	      , _weight (weight)
	      , _target (target)
	{
	} 
	
	action_t action () const {return _action;}
	double weight () const {return _weight;}
	Blob* target () const {return _target;}

	bool matches (const Option& other)
	{
		return (_action == other._action) && (_target == other._target);
	}
private:
	action_t _action;
	double _weight;
	Blob* _target;
};

#endif
