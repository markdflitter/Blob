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
	
	std::string _name;

	unsigned int _lifespan;
	unsigned int _currentAge;	
	
	unsigned int _starvationLevel;
	double _currentHunger;
	
	unsigned int _greatestMaxHP;
	unsigned int _currentHP;
	
	double _wanderingSpeed;
        double _runningSpeed;
	
	double _smell;
	
	unsigned int _damage;
		
	unsigned int _endurance;
	double _aggression;
	unsigned int _size;

	std::vector<Pt<double>> _points;
	
	double _previousAngleInRadians = 0;

	std::string _state;

	unsigned int _fatigue;
	bool _tired;

	bool _dead;

	std::function<double(double)> _moveDirectionFn;
	std::function<double(double)> _aggressionFn;

	double scalePropertyByHPRatio (double property) const;
	double propertyScalingFactorDueToAge () const;
	double propertyScalingFactorDueToHunger () const;
	unsigned int currentMaxHP () const;
	
	void setHP (unsigned int newHP);
	void kill ();
	};

#endif

