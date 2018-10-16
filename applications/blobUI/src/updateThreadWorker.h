#ifndef INCLUDED_UPDATETHREADWORKER_H
#define INCLUDED_UPDATETHREADWORKER_H

#include <thread>
#include <atomic>
#include <QtWidgets>

class UpdateThreadWorker
{
public:
	UpdateThreadWorker () {}
	
	UpdateThreadWorker (const UpdateThreadWorker&) = delete;
	UpdateThreadWorker& operator= (const UpdateThreadWorker&) = delete;
	UpdateThreadWorker (UpdateThreadWorker&&) = delete;
	UpdateThreadWorker& operator= (UpdateThreadWorker&&) = delete;

	std::thread start (QWidget* target);
	void stop ();
private:
	void work(QWidget* target);
	std::atomic<bool> _stopped;
};

#endif

