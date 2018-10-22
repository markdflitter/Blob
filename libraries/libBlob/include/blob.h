#ifndef INCLUDED_BLOB_H
#define INCLUDED_BLOB_H

#include <createBlob.h>
#include <pt.h>
#include <action.h>

#include <functional>
#include <string>
#include <vector>
#include <memory>

class BlobImpl;
	
class Blob 
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

	unsigned int greatestDamage () const;
	unsigned int currentDamage () const;
	
	unsigned int endurance () const;
	double currentFatigue () const;
 	
	unsigned int currentSize () const;
	
	double currentAggression () const;
	
	const std::vector<Pt<double>>& history () const;

	// state
	std::string state () const;
	bool isTired () const;
	bool isDead () const;
	
	//helper functions
	double distance (const Blob& other) const;
	double angle (const Blob& other) const;
	bool isInRange (const Blob& other, double range) const;
	bool isInSameSquare (const Blob& other) const;
	bool canSmell (const Blob& other) const;
	
	// helpers for drawing
	double alpha () const;
//HERE
	// iteraction
	std::shared_ptr <Action> chooseNextAction (std::vector<Blob>& blobs);
	void kill ();
private:
	std::shared_ptr <BlobImpl> _impl;
};

#endif

