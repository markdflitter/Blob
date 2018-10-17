#include "fieldThreadWorker.h"
#include <chrono>
#include <blob.h>
#include <vector>
#include <random>

void createHunters (std::vector<Blob>& blobs, size_t count)
{
	std::mt19937 generator (std::random_device {}());
	std::normal_distribution<> wander_angle_generator (40, 4);
	std::uniform_real_distribution<> x_ordinate_generator (-Blob::WORLD_SIZE ().x (), Blob::WORLD_SIZE ().x ());
	std::uniform_real_distribution<> y_ordinate_generator (-Blob::WORLD_SIZE ().y (), Blob::WORLD_SIZE ().y ());
	std::normal_distribution<> speed_generator (1.5, 0.3);
	std::normal_distribution<> running_speed_multiplier_generator (1.5, 0.5);
	std::normal_distribution<> smell_generator (100, 40);
	std::normal_distribution<> aggression_generator (1.0, 0.25);
	std::normal_distribution<> lifespan_generator (10000, 8000);
	std::normal_distribution<> maxHunger_generator (2000, 500);
	std::normal_distribution<> other_generator (100, 20);
	
	for (size_t n = 0; n < count; ++n)
	{
		double speed = std::max (speed_generator (generator), 0.1);
		double runningSpeed = std::max (running_speed_multiplier_generator (generator), 1.0) * speed;
		double damage = std::max (0.0, other_generator (generator));
		blobs.push_back (CreateBlob ()
				 .name ("hunter_" + std::to_string (n))
				 .moveDirectionFn (RandomMove (std::random_device {} (), wander_angle_generator (generator)))
				 .position (make_pt (x_ordinate_generator (generator), y_ordinate_generator (generator)))
				 .speed (speed)
				 .runningSpeed (runningSpeed)
				 .smell (smell_generator (generator))
				 .HP (std::max (0.0, other_generator (generator)))
				 .endurance (std::max (0.0, other_generator (generator)))
				 .aggression (std::min (1.0, std::max (-1.0, aggression_generator (generator))))
				 .lifespan (std::max (100.0, lifespan_generator (generator)))
				 .maxHunger (maxHunger_generator (generator))
				 .size (maxHunger_generator (generator))
				 .aggressionFn (RandomAggression (std::random_device {} ()))
				 .damage (damage));
	}
}

void createSheep (std::vector<Blob>& blobs, size_t count)
{
	std::mt19937 generator (std::random_device {}());
	std::normal_distribution<> wander_angle_generator (40, 4);
	std::uniform_real_distribution<> x_ordinate_generator (-Blob::WORLD_SIZE ().x (), Blob::WORLD_SIZE ().x ());
	std::uniform_real_distribution<> y_ordinate_generator (-Blob::WORLD_SIZE ().y (), Blob::WORLD_SIZE ().y ());
	std::normal_distribution<> speed_generator (1.8, 0.4);
	std::normal_distribution<> running_speed_multiplier_generator (1.6, 0.5);
	std::normal_distribution<> smell_generator (120, 50);
	std::normal_distribution<> aggression_generator (-1.0, 0.25);
	std::normal_distribution<> lifespan_generator (10000, 8000);
	std::normal_distribution<> maxHunger_generator (2500, 500);
	std::normal_distribution<> other_generator (30, 20);
	
	for (size_t n = 0; n < count; ++n)
	{
		double speed = std::max (speed_generator (generator), 0.1);
		double runningSpeed = std::max (running_speed_multiplier_generator (generator), 1.0) * speed;
		double damage = std::max (0.0, other_generator (generator));
		blobs.push_back (CreateBlob ()
				 .name ("sheep_" + std::to_string (n))
				 .moveDirectionFn (RandomMove (std::random_device {} (), wander_angle_generator (generator)))
				 .position (make_pt (x_ordinate_generator (generator), y_ordinate_generator (generator)))
				 .speed (speed)
				 .runningSpeed (runningSpeed)
				 .smell (smell_generator (generator))
				 .HP (std::max (0.0, other_generator (generator)))
				 .endurance (std::max (0.0, other_generator (generator)))
				 .aggression (std::min (1.0, std::max (-1.0, aggression_generator (generator))))
				 .lifespan (std::max (100.0, lifespan_generator (generator)))
				 .maxHunger (maxHunger_generator (generator))
				 .size (maxHunger_generator (generator))
				 .aggressionFn (RandomAggression (std::random_device {} ()))
				 .damage (damage));
	}
}

void createRandomBlobs (std::vector<Blob>& blobs, size_t count)
{
	std::mt19937 generator (std::random_device {}());
	std::normal_distribution<> wander_angle_generator (40, 4);
	std::uniform_real_distribution<> x_ordinate_generator (-Blob::WORLD_SIZE ().x (), Blob::WORLD_SIZE ().x ());
	std::uniform_real_distribution<> y_ordinate_generator (-Blob::WORLD_SIZE ().y (), Blob::WORLD_SIZE ().y ());
	std::normal_distribution<> speed_generator (1.2, 0.5);
	std::normal_distribution<> running_speed_multiplier_generator (1.5, 0.5);
	std::normal_distribution<> smell_generator (120, 70);
	std::normal_distribution<> aggression_generator (0.0, 0.5);
	std::normal_distribution<> lifespan_generator (10000, 8000);
	std::normal_distribution<> maxHunger_generator (2000, 500);
	std::normal_distribution<> other_generator (50, 40);
	
	for (size_t n = 0; n < count; ++n)
	{
		double speed = std::max (speed_generator (generator), 0.1);
		double runningSpeed = std::max (running_speed_multiplier_generator (generator), 1.0) * speed;
		double damage = std::max (0.0, other_generator (generator));
		blobs.push_back (CreateBlob ()
				 .name ("random_" + std::to_string (n))
				 .moveDirectionFn (RandomMove (std::random_device {} (), wander_angle_generator (generator)))
				 .position (make_pt (x_ordinate_generator (generator), y_ordinate_generator (generator)))
				 .speed (speed)
				 .runningSpeed (runningSpeed)
				 .smell (smell_generator (generator))
				 .HP (std::max (0.0, other_generator (generator)))
				 .endurance (std::max (0.0, other_generator (generator)))
				 .aggression (std::min (1.0, std::max (-1.0, aggression_generator (generator))))
				 .lifespan (std::max (100.0, lifespan_generator (generator)))
				 .maxHunger (maxHunger_generator (generator))
				 .size (maxHunger_generator (generator))
				 .aggressionFn (RandomAggression (std::random_device {} ()))
				 .damage (damage));
	}
}

Blob createTheHandOfGod ()
{
	std::mt19937 generator (std::random_device {}());
	std::normal_distribution<> wander_angle_generator (40, 8);
	std::uniform_real_distribution<> x_ordinate_generator (-Blob::WORLD_SIZE ().x (), Blob::WORLD_SIZE ().x ());
	std::uniform_real_distribution<> y_ordinate_generator (-Blob::WORLD_SIZE ().y (), Blob::WORLD_SIZE ().y ());
	
	Blob result = CreateBlob ().name ("TheHandOfGod")
				  .moveDirectionFn (RandomMove (std::random_device {} (), wander_angle_generator (generator)))
				  .position (make_pt (x_ordinate_generator (generator), y_ordinate_generator (generator)))
				  .speed (1.0)
				  .runningSpeed (2.0)
				  .smell (400.0)
				  .HP (200)
				  .endurance (100)
				  .aggression (1)
				  .lifespan (50000)
				  .maxHunger (4000U)
				  .size (10000U)
				  .aggressionFn ([](double a) {return a;})
				  .damage (200);
	
	return result;
}

std::thread FieldThreadWorker::start (SetBlobs* target)
{
	_stopped = false;
	return std::thread ([this, target] {this->work (target);});
}

void FieldThreadWorker::work (SetBlobs* target)
{
	using namespace std::literals;
	using namespace std::chrono_literals;
	using namespace std::chrono;

	std::vector<Blob> blobs;
	createRandomBlobs (blobs, 10);
	createRandomBlobs (blobs, 140);
	createSheep (blobs, 75);
	createHunters (blobs, 75);
	blobs.push_back (createTheHandOfGod ());

	unsigned int t = 0;
	while (!_stopped)
	{
		high_resolution_clock::time_point start = high_resolution_clock::now ();
		
		std::time_t time = std::time (nullptr);
		numCalcsThisSecond++;
		if (time != lastReportTime)
		{
			lastReportTime = time;
			if (_rateListener)
			{
				_rateListener->setCalcRate (numCalcsThisSecond);
			}
			numCalcsThisSecond = 0U;
		}

		std::vector<std::shared_ptr <Action>> actions;
		for (auto& b : blobs)
		{
			actions.push_back (b.chooseNextAction (blobs));
		}

		for (auto a : actions)
		{
			a->apply ();
		}	
		
		std::shared_ptr <std::vector<Blob>> newState = std::make_shared <std::vector<Blob>> (blobs);
		target->setBlobs (newState, t++);
		
		high_resolution_clock::time_point end = high_resolution_clock::now ();
	
		duration<double, std::milli> timeTaken = end - start;
		
		if (timeTaken.count () < 20)
		{
			std::this_thread::sleep_for (std::chrono::milliseconds (static_cast <int> (20.0 - timeTaken.count ())));
		}
	}
}

void FieldThreadWorker::stop ()
{
	_stopped = true;
}

