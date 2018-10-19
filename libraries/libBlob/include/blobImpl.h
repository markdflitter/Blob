#ifndef INCLUDED_BLOBIMPL_H
#define INCLUDED_BLOBIMPL_H

#include <functional>
#include <string>
#include <vector>
#include <pt.h>
#include <createBlob.h>
	
class BlobImpl
{
public:
	BlobImpl (const CreateBlob& params);
	
	Pt<double> position () const;
 
	double propertyScalingFactorDueToAge () const;
	double propertyScalingFactorDueToHunger () const;
	unsigned int maxHP () const;
	
	void setHP (unsigned int newHP);
	void kill ();
	
	std::vector<Pt<double>> _points;

	std::string _name;
	double _wanderingSpeed;
        double _runningSpeed;
	double _baseSmell;
	unsigned int _baseHP;
	unsigned int _HP;
	unsigned int _endurance;
	double _aggression;
	unsigned int _lifespan;
	unsigned int _baseDamage;
	unsigned int _maxHunger;
	double _hunger;
	unsigned int _size;

	double _previousAngleInRadians = 0;

	std::string _state;

	unsigned int _fatigue;
	bool _tired;

	unsigned int _age;	
	bool _dead;

	std::function<double(double)> _moveDirectionFn;
	std::function<double(double)> _aggressionFn;
};

#endif

