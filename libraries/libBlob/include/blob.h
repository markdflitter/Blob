#ifndef INCLUDED_BLOB_H
#define INCLUDED_BLOB_H

#include "createBlob.h"
#include "pt.h"
#include "action.h"
#include "movement.h"
#include "fight.h"
#include "eat.h"
#include "option.h"

#include <functional>
#include <iostream>
#include <string>
#include <vector>
#include <memory>

class BlobImpl;
	
class Blob : public Moveable, public Target, public Eater, public Food
{
public:
	// constructors
	Blob (const CreateBlob& params);
	Blob (const Blob& other);
	
	//static properties
	static Pt<double> WORLD_SIZE ();
	
	// impl loophole
	std::shared_ptr <BlobImpl> getImpl ();
	
	// basic property accessors
	std::string name () const;

 	unsigned int lifespan () const;
	unsigned int currentAge () const;

	unsigned int starvationLevel () const;
 	double currentHunger () const;

	unsigned int greatestMaxHP () const;
	unsigned int currentMaxHP () const;
	unsigned int currentHP () const;
	
	double greatestWanderingSpeed () const;
	double greatestRunningSpeed () const;
	double currentWanderingSpeed () const;
	double currentRunningSpeed () const;
	
	double currentSmell () const;

	unsigned int baseDamage () const;
	unsigned int damage () const;
	
	unsigned int endurance () const;
	double fatigue () const;

	double aggression () const;
 	
	unsigned int size () const;

	std::string state () const;

	bool isDead () const;

	const std::vector<Pt<double>>& history () const;

	double distance (const Blob& other) const;
	double angle (const Blob& other) const;
	
	bool isInRange (const Blob& other, double range) const;
	bool isInSameSquare (const Blob& other) const;
	bool canSmell (const Blob& other) const;

	// helpers for drawing
	double fade () const;
		
	std::shared_ptr <Action> chooseNextAction (std::vector<Blob>& blobs);

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

	// private	
	void limitHPtoMax (unsigned int previousDamage);
	
	void growOlder ();
	void getHungrier (double amount);
	
	std::shared_ptr <Action> createActionDead ();
        std::shared_ptr <Action> createActionWander ();
	std::shared_ptr <Action> createActionFlee (const Blob& target);
	std::shared_ptr <Action> createActionHunt (const Blob& target);
	std::shared_ptr <Action> createActionFight (Blob& target);
	std::shared_ptr <Action> createActionAttack (Blob& target);
	std::shared_ptr <Action> createActionEat (Blob& food);

	static double relativeDifference (double v1, double v2);
	double inflictDamageWeight (const Blob& b) const;
	double hungerWeight (const Blob& b) const;
	double avoidDamageWeight (const Blob& b) const;
	double distanceWeight (const Blob& b) const;
	double attackWeight (const Blob& b) const;
	double fleeWeight (const Blob& b) const;

	std::vector<Option> findOptions (std::vector<Blob>& others) const;
	std::vector<Option> prioritiseOptions (const std::vector <Option>& options);
	Option selectBestOption (const std::vector <Option>& options);
	Option chooseBestOption (std::vector<Blob>& blobs);
private:
	std::shared_ptr <BlobImpl> _impl;
};

#endif

