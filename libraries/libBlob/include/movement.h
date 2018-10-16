#ifndef INCLUDED_MOVEMENT_H
#define INCLUDED_MOVEMENT_H

#include <action.h>
#include <string>
#include <iostream>
#include <iomanip>
#include <math.h>

class Moveable
{
public:
	virtual ~Moveable () {}
	virtual void move (double speed, double angleInRadians, const std::string& reason) = 0;
};

class Movement : public Action
{
	public:
		Movement (Moveable* target, const std::string& reason, double speed, double angleInRadians) :
			_target (target)
			, _reason (reason)
			, _speed (speed)
			, _angleInRadians (angleInRadians)
		{
		}

		bool operator== (const Movement& m) const
		{
			return (_target == m._target) 
				&& (_reason == m._reason)
				&& (_speed == m._speed)
				&& (_angleInRadians == m._angleInRadians);
		}	

		bool operator!= (const Movement& m) const
		{
			return !(*this == m);
		}

		void apply ();

		friend std::ostream& operator<< (std::ostream& s, const Movement& m);
	public:
		Moveable* _target;
		std::string _reason; 
		double _speed;
		double _angleInRadians;
};

inline std::ostream& operator<< (std::ostream& s, const Movement& m)
{
	s << std::fixed << std::setprecision (5) << m._reason << "," << m._speed << "," << 360 * m._angleInRadians / (2 * M_PI);
	return s; 
}

inline void Movement::apply ()
{
	_target->move (_speed, _angleInRadians, _reason);
}

#endif
