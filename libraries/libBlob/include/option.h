#ifndef INCLUDED_OPTION_H
#define INCLUDED_OPTION_H

enum action_t {attack, flee, wander, eat};

class BlobImpl;

class Option 
{
public:
	Option (action_t action, double weight, BlobImpl* target) :
		_action (action)
	      , _weight (weight)
	      , _target (target)
	{
	} 
	
	action_t action () const {return _action;}
	double weight () const {return _weight;}
	BlobImpl* target () const {return _target;}

	bool matches (const Option& other)
	{
		return (_action == other._action) && (_target == other._target);
	}
private:
	action_t _action;
	double _weight;
	BlobImpl* _target;
};

#endif
