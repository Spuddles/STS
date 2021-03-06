#include "Logger.h"
#include "WinHTTPRequest.h"
#include "CURLRequest.h"
#include "BinanceAPI.h"
#include "AccountInfo.h"
#include "Helpers.h"
#include <iostream>
#include <cstdlib>
#include "IStreamReceiver.h"

using namespace STS;

class Receiver : public IStreamReceiver
{
public:
	void dataReceived(const char *data)
	{
		std::cout << data << std::endl;
	}
};

int main()
{
	AccountInfo ai;

	CURLRequest	cr;
	BinanceAPI bapi(&cr);
	bapi.setKeys("a",
		"b");


	// Get a listen key
	std::string key;
	if (!bapi.getListenKey(key))
	{
		Log(CRITICAL, "Hello");
		std::cout << "Failed to get the listening key" << std::endl;
		return 1;
	}

	// Open a websocket connection to monitor our account
	WinHTTPRequest ws;
	std::string url = "/ws/" + key;
	WCHAR widestr[256] = { 0 };
	WCHAR *pws = widestr;
	for (auto c : url)
	{
		mbtowc(pws++, &c, 2);
	}
	const WCHAR	*p1 = L"/ws/poobear";

	//	ws.initialise(L"/ws/poebtc@kline_1m");
	ws.initialise(widestr);

	Receiver r;
	ws.receiveWebSocketStream(&r);

	// Now run a loop that triggers every 1 minute to check our strategy
	// and send any orders we might create
	int hours = -1, minutes = -1, seconds = -1;
	while (true)
	{
		while (seconds != 0)
		{
			Helpers::sleepMS(1000);
			Helpers::getCurrentTime(hours, minutes, seconds);
		}

		std::cout << "Processing at " << hours << ":" << minutes << ":" << seconds++ << std::endl;

		//		bapi.getAccountInformation(ai);

				// Send key update every 15 minutes
		if (minutes % 5 == 0)
		{
			bapi.keepAliveListenKey(key);
		}
	}
	bapi.closeListeningKey(key);
	return 0;
}
