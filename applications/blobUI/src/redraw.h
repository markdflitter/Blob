#ifndef INCLUDED_REDRAW_H
#define INCLUDED_REDRAW_H

class Redraw
{
public:
	virtual ~Redraw () {};
	virtual void redrawPixmap () = 0;
};

#endif
