#ifndef INCLUDED_EAT_H
#define INCLUDED_EAT_H

#include <action.h>
#include <iostream>

class Food
{
public:
	virtual ~Food() {}
	virtual unsigned int takeABite (unsigned int biteSize) = 0;
};

class Eater
{
public:
	virtual ~Eater () {}
	virtual void eat (Food* food, const std::string& state) = 0;
};

class Eat : public Action
{
	public:
		Eat (Eater* eater, Food* food, const std::string& state) :
			_eater (eater)
		      , _food (food)
		      , _state (state)
		{
		}

		bool operator== (const Eat& a) const
		{
			return (_eater == a._eater) && (_food == a._food); 
		}	

		bool operator!= (const Eat& a) const
		{
			return !(*this == a); 
		}

		void apply ();
	public:
		Eater* _eater;
		Food* _food;
		std::string _state;
};

inline void Eat::apply ()
{
	_eater->eat (_food, _state);
}

#endif
