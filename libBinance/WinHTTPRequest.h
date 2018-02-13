#pragma once
#include <string>
#include <Windows.h> 
#include <WinHttp.h> 
#include "IStreamReceiver.h"

#define WSS_BASE_ADDR	L"stream.binance.com"
#define WSS_BASE_PORT	(9443)


class WinHTTPRequest
{
public:
				WinHTTPRequest();
	virtual		~WinHTTPRequest();

				// stream should be /ws/<listenkey>
	bool		initialise(const WCHAR *stream);
	void		receiveWebSocketStream(IStreamReceiver *receiver);

private:

	bool		openSession();
	bool		httpConnect();
	bool		openRequest(const WCHAR *stream);
	bool		upgradeToWebSocket();
	bool		sendUpgradeRequest();
	bool		receiveResponse();
	bool		checkForSuccessfullRequest();
	bool		completeUpgrade();

	static void	receiveDataStatic(void *pThis);

	DWORD		m_dwError;
	HINTERNET	m_hSessionHandle;
	HINTERNET	m_hConnectionHandle;
	HINTERNET	m_hRequestHandle;
	HINTERNET	m_hWebSocketHandle;

	// Callback class for sending our data
	IStreamReceiver	*m_receiver;
};

