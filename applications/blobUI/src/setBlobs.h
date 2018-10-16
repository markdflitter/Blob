#ifndef INCLUDED_SETBLOBS_H
#define INCLUDED_SETBLOBS_H

#include <blob.h>
#include <vector>

class SetBlobs
{
public:
	virtual ~SetBlobs() {}
	virtual void setBlobs (std::shared_ptr <std::vector<Blob>> blobs, unsigned int time) = 0;
};

#endif
