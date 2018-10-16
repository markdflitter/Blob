#include "redrawThreadWorker.h"
#include <chrono>

std::thread RedrawThreadWorker::start (Redraw* target)
{
	_stopped = false;
	return std::thread ([this, target] {this->work (target);});
}

void RedrawThreadWorker::stop ()
{
	_stopped = true;
}

void RedrawThreadWorker::work (Redraw* target)
{
	using namespace std::literals;
	using namespace std::chrono_literals;

	while (!_stopped)
	{
		target->redrawPixmap ();
		std::this_thread::sleep_for (20ms);
	}
}


