#ifndef INCLUDED_REDRAWTHREADWORKER_H
#define INCLUDED_REDRAWTHREADWORKER_H

#include <thread>
#include <atomic>
#include <QtWidgets>
#include "redraw.h"

class RedrawThreadWorker
{
public:
	RedrawThreadWorker () {}
	
	RedrawThreadWorker (const RedrawThreadWorker&) = delete;
	RedrawThreadWorker& operator= (const RedrawThreadWorker&) = delete;
	RedrawThreadWorker (RedrawThreadWorker&&) = delete;
	RedrawThreadWorker& operator= (RedrawThreadWorker&&) = delete;

	std::thread start (Redraw* target);
	void stop ();
private:
	void work(Redraw* target);
	std::atomic<bool> _stopped;
};

#endif

