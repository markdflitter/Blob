#ifndef INCLUDED_PT_H
#define INCLUDED_PT_H

template <typename T>
class Pt {
public:
  Pt () : _x (0), _y (0) {}
  Pt (T x, T y) : _x (x), _y (y) {}
  
  T x () const {return _x;}
  T y () const {return _y;}

  T& x () {return _x;}
  T& y () {return _y;}
private:
  T _x;
  T _y;
};

template <typename T> Pt<T> make_pt (T p1, T p2)
{
  return Pt<T> (p1, p2);
} 
#endif

