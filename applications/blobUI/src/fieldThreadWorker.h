#ifndef INCLUDED_FIELDTHREADWORKER_H
#define INCLUDED_FIELDTHREADWORKER_H

#include <thread>
#include <atomic>
#include "setBlobs.h"
#include "rateListener.h"

class FieldThreadWorker
{
	public:
		FieldThreadWorker () {}
	
		FieldThreadWorker (const FieldThreadWorker&) = delete;
		FieldThreadWorker& operator= (const FieldThreadWorker&) = delete;
		FieldThreadWorker (FieldThreadWorker&&) = delete;
		FieldThreadWorker& operator= (FieldThreadWorker&&) = delete;

		std::thread start (SetBlobs* target);
		void stop ();
		
		void setRateListener (RateListener* rateListener) {_rateListener = rateListener;}
	private:
		void work(SetBlobs* target);
		std::atomic<bool> _stopped;
		
		unsigned int numCalcsThisSecond = 0;
		std::time_t lastReportTime = std::time (nullptr);

		RateListener* _rateListener = 0;
};

#endif

