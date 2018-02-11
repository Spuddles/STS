#pragma once
#include <string>
#include <Windows.h> 
#include <WinHttp.h> 

#define WSS_BASE_ADDR	L"stream.binance.com"
#define WSS_BASE_PORT	(9443)


class WinHTTPRequest
{
public:
	WinHTTPRequest();
	virtual ~WinHTTPRequest();

	bool	initialise(const WCHAR *stream);

private:

	bool		openSession();
	bool		httpConnect();
	bool		openRequest(const WCHAR *stream);
	bool		upgradeToWebSocket();
	bool		sendUpgradeRequest();
	bool		receiveResponse();
	bool		checkForSuccessfullRequest();
	bool		completeUpgrade();

	void		receiveWebSocketStream();

	DWORD		m_dwError;
	HINTERNET	m_hSessionHandle;
	HINTERNET	m_hConnectionHandle;
	HINTERNET	m_hRequestHandle;
	HINTERNET	m_hWebSocketHandle;
};

