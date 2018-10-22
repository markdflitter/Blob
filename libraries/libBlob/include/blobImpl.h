#ifndef INCLUDED_BLOBIMPL_H
#define INCLUDED_BLOBIMPL_H

#include <functional>
#include <string>
#include <vector>
#include <memory>
#include <pt.h>
#include <createBlob.h>
#include <action.h>
#include <option.h>
#include <movement.h>
#include <fight.h>
#include <eat.h>

class BlobImpl : public Moveable, public Target, public Eater, public Food
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

	double distance (BlobImpl* other) const;
	double angle (BlobImpl* other) const;
	bool isInRange (BlobImpl* other, double range) const;
	bool isInSameSquare (BlobImpl* other) const;
	bool canSmell (BlobImpl* other) const;
		
	double currentWanderingSpeed () const;
	double currentRunningSpeed () const;
	
	double currentSmell () const;

	double scalePropertyByHPRatio (double property) const;
	double propertyScalingFactorDueToAge () const;
	double propertyScalingFactorDueToHunger () const;
	unsigned int currentMaxHP () const;
	
	void setHP (unsigned int newHP);
	void kill ();

	void limitHPtoMax (unsigned int previousDamage);
	
	void growOlder ();
	void getHungrier (double amount);

	static Pt<double> WORLD_SIZE ();
	
	unsigned int currentDamage () const;
	unsigned int currentSize () const;

	std::shared_ptr <Action> createActionDead ();
        std::shared_ptr <Action> createActionWander ();
	std::shared_ptr <Action> createActionFlee (BlobImpl* target);
	std::shared_ptr <Action> createActionHunt (BlobImpl* target);
	std::shared_ptr <Action> createActionFight (BlobImpl* target);
	std::shared_ptr <Action> createActionAttack (BlobImpl* target);
	std::shared_ptr <Action> createActionEat (BlobImpl* food);

	static double relativeDifference (double v1, double v2);
	double inflictDamageWeight (BlobImpl* b) const;
	double hungerWeight (BlobImpl* b) const;
	double avoidDamageWeight (BlobImpl* b) const;
	double distanceWeight (BlobImpl* b) const;
	double attackWeight (BlobImpl* b) const;
	double fleeWeight (BlobImpl* b) const;

	std::vector<Option> findOptions (std::vector<BlobImpl*> others) const;
	std::vector<Option> prioritiseOptions (const std::vector <Option>& options);
	Option selectBestOption (const std::vector <Option>& options);
	Option chooseBestOption (std::vector<BlobImpl*> blobs);

	// Moveable
        void move (double speed, double angleInRadians, const std::string& newState);

	// Target
	void takeDamage (unsigned int damage);
	void inflictDamage (Target* target, const std::string& state);
	void retaliate (Target* target);

	// Eater
	void eat (Food* food, const std::string& state);

	// Food
	unsigned int takeABite (unsigned int biteSize);
	std::shared_ptr <Action> chooseNextAction (std::vector<BlobImpl*> blobs);
};

#endif

