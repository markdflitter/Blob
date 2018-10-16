#include "updateThreadWorker.h"
#include <chrono>

std::thread UpdateThreadWorker::start (QWidget* target)
{
	_stopped = false;
	return std::thread ([this, target] {this->work (target);});
}

void UpdateThreadWorker::stop ()
{
	_stopped = true;
}

void UpdateThreadWorker::work (QWidget* target)
{
	using namespace std::literals;
	using namespace std::chrono_literals;

	while (!_stopped)
	{
		std::this_thread::sleep_for (20ms);
		target->update ();
	}
}


