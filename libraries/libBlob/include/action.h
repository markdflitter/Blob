#ifndef INCLUDED_ACTION_H
#define INCLUDED_ACTION_H

class Action 
{
public:
	virtual ~Action () {};
	virtual void apply () = 0;
};

#endif
