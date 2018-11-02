#ifndef INCLUDED_BLOB_H
#define INCLUDED_BLOB_H

#include <functional>
#include <algorithm>
#include <iostream>
#include <math.h>
#include <string>
#include <vector>
#include <sstream>
#include <memory>
#include <pt.h>
#include <action.h>
#include <movement.h>
#include <fight.h>
#include <eat.h>
#include <rnd.h>
#include "option.h"
#include <assert.h>
#include "createBlob.h"

class BlobImpl;
	
class Blob : public Moveable, public Target, public Eater, public Food
{
public:
	Blob (const CreateBlob& params);
	Blob (const Blob& other);

	std::shared_ptr <BlobImpl> getImpl ();
	
	std::string name () const;
	double x () const; 
	double y () const;
	double baseSpeed () const;
	double speed () const;
	double runningSpeed () const;
	double smell () const;
	unsigned int baseHP () const;
	unsigned int HP () const;
	unsigned int maxHP () const;
	unsigned int baseDamage () const;
	unsigned int damage () const;
	unsigned int endurance () const;
	double aggression () const;
 	unsigned int maxHunger () const;
 	double hunger () const;
 	unsigned int size () const;
	unsigned int lifespan () const;
	unsigned int age () const;
	std::string state () const;

	double fatigue () const;
	bool isTired () const;

	bool isDead () const;

	const std::vector<Pt<double>>& history () const;

	double distance (const Blob& other) const;
	bool isInRange (const Blob& other, double range) const;
	bool isInSameSquare (const Blob& other) const;
	bool canSmell (const Blob& other) const;

	double angle (const Blob& other) const;

	void limitHPtoMax (unsigned int previousDamage);
	
	void growOlder ();
	void getHungrier (double amount);
	
	void takeDamage (unsigned int damage);
	void inflictDamage (Target* target, const std::string& state);
	void retaliate (Target* target);

        void move (double speed, double angleInRadians, const std::string& newState);
      
	void eat (Food* food, const std::string& state);
	unsigned int takeABite (unsigned int biteSize);
 
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
	std::shared_ptr <Action> chooseNextAction (std::vector<Blob>& blobs);

	friend std::ostream& operator<<(std::ostream& s, const Blob& b);
	
	static Pt<double> WORLD_SIZE ();
private:
	std::shared_ptr <BlobImpl> _impl;
};

#endif

