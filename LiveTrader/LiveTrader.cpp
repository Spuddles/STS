#include "WinHTTPRequest.h"
#include "CURLRequest.h"
#include "BinanceAPI.h"
#include <iostream>

int main()
{
	CURLRequest	cr;
	BinanceAPI bapi(&cr);

//	std::string key = bapi.getListenKey();

/*	WinHTTPRequest ws;
	ws.initialise(L"/ws/poebtc@kline_1m");
	ws.initialise(L"/stream?streams=bnbbtc@kline_1m/neobtc@kline_1m/");*/

    return 0;
}

