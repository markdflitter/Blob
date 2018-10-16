#ifndef RATE_LISTENER_INCLUDED
#define RATE_LISTENER_INCLUDED

class RateListener
{
public:
	virtual ~RateListener () {}
	virtual void setRedrawRate (unsigned int redrawRate) = 0;
	virtual void setCalcRate (unsigned int calcRate) = 0;
};

#endif
