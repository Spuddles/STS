#include "WebSocketRequest.h"
#include "BinanceAPI.h"
#include <iostream>

int main()
{
	WebSocketRequest wsr;
	BinanceAPI bapi(&wsr);

	uint64_t timestamp=0LL;
	if (!bapi.getTime(timestamp))
	{
		std::cout << "Failed to get timestamp from Binance" << std::endl;
	}

    return 0;
}

