#ifndef INCLUDED_CREATEBLOB_H
#define INCLUDED_CREATEBLOB_H

#include <functional>
#include <algorithm>
#include <iostream>
#include <string>
#include <pt.h>
	
class CreateBlob 
{
public:
	CreateBlob ();
	CreateBlob name (const std::string& name);
	
	CreateBlob lifespan (unsigned int lifespan);
	
	CreateBlob starvationLevel (unsigned int starvationLevel);
	
	CreateBlob position (const Pt<double>& position);
	CreateBlob speed (double speed);
	CreateBlob runningSpeed (double runningSpeed);
	CreateBlob smell (double smell);
	CreateBlob HP (unsigned int HP);
	CreateBlob endurance (unsigned int endurance);
	CreateBlob aggression (double aggression);
	CreateBlob damage (unsigned int damage);
	CreateBlob size (unsigned int size);
	CreateBlob moveDirectionFn (std::function<double(double)> moveDirectionFn);
	CreateBlob aggressionFn (std::function<double(double)> aggressionFn);
private:
	friend class Blob;
	friend class BlobImpl;
	
	std::string _name;
	
	unsigned int _lifespan;
	
	unsigned int _starvationLevel;

	unsigned int _HP;
	
	Pt<double> _position;
	double _speed;
        double _runningSpeed;
	double _smell;
	unsigned int _endurance;
	double _aggression;
	unsigned int _damage;
	unsigned int _size;

	std::function<double(double)> _moveDirectionFn;
	std::function<double(double)> _aggressionFn;
};

#endif
