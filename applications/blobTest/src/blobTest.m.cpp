#include <iostream>
#include <random>
#include <blob.h>

int main (void) 
{
	Blob b0 = CreateBlob ().name ("b0").speed (0.1).lifespan (2000U)
			       .moveDirectionFn ([](double i) {return i + M_PI/1000;});
	Blob b1 = CreateBlob ().name ("b1").position (make_pt (-30.0, -30.0)).speed (0.25).lifespan (2000U)
			       .moveDirectionFn ([](double i) {return i + M_PI/1000;});
	Blob b2 = CreateBlob ().name ("b2").position (make_pt (30.0, -30.0)).speed (0.1).lifespan (2000U)
			       .moveDirectionFn ([](double i) {return i + M_PI/1000;});
	Blob b3 = CreateBlob ().name ("b3").position (make_pt (-30.0, 30.0)).speed (0.3).lifespan (2000U)
			       .moveDirectionFn ([](double i) {return i + M_PI/1000;});
	Blob b4 = CreateBlob ().name ("b4").position (make_pt (30.0, 30.0)).speed (0.35).lifespan (2000U)
			       .moveDirectionFn ([](double i) {return i + M_PI/1000;});
	
        std::vector <Blob> blobs {b0, b1, b2, b3, b4};
	
	for (int n = 0; n < 2000; n++)
	{
		std::cout << n + 1;

		std::vector<std::shared_ptr <Action>> moves;
		for (auto& b : blobs)
		{
			moves.push_back (b.chooseNextAction (blobs));
		}

		for (auto m : moves)
		{
			std::cout << ",";
			std::cout << m;
			m->apply ();
		}	

		for (auto& b : blobs)
		{
			std::cout << ",";
			std::cout << b;
		}
	
		std::cout << '\n';	
	}

	return 0;
}

