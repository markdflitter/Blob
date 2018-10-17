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
	CreateBlob position (const Pt<double>& position);
	CreateBlob speed (double speed);
	CreateBlob runningSpeed (double runningSpeed);
	CreateBlob smell (double smell);
	CreateBlob HP (unsigned int HP);
	CreateBlob endurance (unsigned int endurance);
	CreateBlob aggression (double aggression);
	CreateBlob lifespan (unsigned int lifespan);
	CreateBlob damage (unsigned int damage);
	CreateBlob maxHunger (unsigned int maxHunger);
	CreateBlob size (unsigned int size);
	CreateBlob moveDirectionFn (std::function<double(double)> moveDirectionFn);
	CreateBlob aggressionFn (std::function<double(double)> aggressionFn);
private:
	friend class Blob;
	
	std::string _name;
	Pt<double> _position;
	double _speed;
        double _runningSpeed;
	double _smell;
	unsigned int _HP;
	unsigned int _endurance;
	double _aggression;
	unsigned int _lifespan;
	unsigned int _damage;
	unsigned int _maxHunger;
	unsigned int _size;

	std::function<double(double)> _moveDirectionFn;
	std::function<double(double)> _aggressionFn;
};

#endif
