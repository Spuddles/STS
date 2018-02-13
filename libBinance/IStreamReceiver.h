#pragma once
#include <string>

class IStreamReceiver
{
public:
	virtual void dataReceived(const char *data) = 0;
};