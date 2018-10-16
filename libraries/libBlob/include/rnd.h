#ifndef INCLUDED_RND_H
#define INCLUDED_RND_H

#include <random>
#include <math.h>

class RandomMove 
{
public:
	RandomMove (unsigned int seed, int stddev) :
		_gen (seed)
	     ,  _dist (0, stddev) 
	{
	}

	double operator ()(double previousMoveDirection) 
	{
		return previousMoveDirection + (2 * M_PI * _dist (_gen) / 360);
	}
private:
	std::mt19937 _gen;
	std::normal_distribution<> _dist;
};

class RandomAggression
{
public:
	RandomAggression (unsigned int seed) :
		_gen (seed)
	     ,  _dist (0.0, 0.5) 
	{
	}

	double operator ()(double aggression) 
	{
		return std::max (-1.0, std::min (1.0, aggression + _dist (_gen)));
	}
private:
	std::mt19937 _gen;
	std::normal_distribution<> _dist;
};

#endif
