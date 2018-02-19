#include "WinHTTPRequest.h"
#include <iostream>
#include <thread>
#include <chrono>

WinHTTPRequest::WinHTTPRequest()
{
}

WinHTTPRequest::~WinHTTPRequest()
{
}

bool WinHTTPRequest::initialise(const WCHAR *stream)
{
	if (!openSession())
		return false;

	if (!httpConnect())
		return false;

	if (!openRequest(stream))
		return false;

	if (!upgradeToWebSocket())
		return false;

	if (!sendUpgradeRequest())
		return false;

	if (!receiveResponse())
		return false;

	if (!checkForSuccessfullRequest())
		return false;

	if (!completeUpgrade())
		return false;

	return true;
}

bool WinHTTPRequest::openSession()
{
	m_hSessionHandle = WinHttpOpen(L"STS stream",
		WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, NULL, NULL, 0);
	if (m_hSessionHandle == NULL)
	{
		m_dwError = GetLastError();
		return false;
	}
	return true;
}

bool WinHTTPRequest::httpConnect()
{
	m_hConnectionHandle = WinHttpConnect(m_hSessionHandle,
		WSS_BASE_ADDR, WSS_BASE_PORT, 0);
	if (m_hConnectionHandle == NULL)
	{
		m_dwError = GetLastError();
		return false;
	}
	return true;
}

bool WinHTTPRequest::openRequest(const WCHAR *stream)
{
	m_hRequestHandle = WinHttpOpenRequest(m_hConnectionHandle,
		L"GET",	stream, NULL, NULL,	NULL, WINHTTP_FLAG_SECURE);
	if (m_hRequestHandle == NULL)
	{
		m_dwError = GetLastError();
		return false;
	}
	return true;
}

bool WinHTTPRequest::upgradeToWebSocket()
{
	BOOL fStatus = WinHttpSetOption(m_hRequestHandle,
		WINHTTP_OPTION_UPGRADE_TO_WEB_SOCKET, NULL,	0);
	if (!fStatus)
	{
		m_dwError = GetLastError();
		return false;
	}
	return true;
}

bool WinHTTPRequest::sendUpgradeRequest()
{
	BOOL fStatus = WinHttpSendRequest(m_hRequestHandle,
		WINHTTP_NO_ADDITIONAL_HEADERS, 0, NULL, 0, 0, 0);
	if (!fStatus)
	{
		m_dwError = GetLastError();
		return false;
	}
	return true;
}

bool WinHTTPRequest::receiveResponse()
{
	BOOL fStatus = WinHttpReceiveResponse(m_hRequestHandle, 0);
	if (!fStatus)
	{
		m_dwError = GetLastError();
		return false;
	}
	return true;
}

/* If we get nothing back after our request to upgrade to a web socket, */
/* we treat that as successful. Otherwise something went wrong and we   */
/* should log it out to help debugging									*/
bool WinHTTPRequest::checkForSuccessfullRequest()
{
	DWORD dwSize;

	if (!WinHttpQueryDataAvailable(m_hRequestHandle, &dwSize))
	{
		m_dwError = GetLastError();
		return false;
	}

	if (dwSize > 0)
	{
		WCHAR *lpOutBuffer = new WCHAR[dwSize + 1];
		DWORD  dwBytesTransferred;

		BOOL bRet = WinHttpReadData(m_hRequestHandle,
			lpOutBuffer, dwSize, &dwBytesTransferred);
		if (bRet == FALSE)
		{
			return false;
		}
		lpOutBuffer[dwBytesTransferred] = 0;
		std::cout << *lpOutBuffer << std::endl;
		return false;
	}
	return true;
}

bool WinHTTPRequest::completeUpgrade()
{
	m_hWebSocketHandle = WinHttpWebSocketCompleteUpgrade(m_hRequestHandle, NULL);
	if (m_hWebSocketHandle == NULL)
	{
		m_dwError = GetLastError();
		return false;
	}
	WinHttpCloseHandle(m_hRequestHandle);
	m_hRequestHandle = NULL;
	return true;
}

void WinHTTPRequest::receiveDataStatic(void *pThis)
{
	WinHTTPRequest *wr = static_cast<WinHTTPRequest*>(pThis);

	char	*lpOutput = new char[65536];
	DWORD	dwBytesTransferred;
	WINHTTP_WEB_SOCKET_BUFFER_TYPE eBufferType = WINHTTP_WEB_SOCKET_BINARY_MESSAGE_BUFFER_TYPE;
	while (true)
	{
		// This call is blocking so it will wait for data
		wr->m_dwError = WinHttpWebSocketReceive(wr->m_hWebSocketHandle,
			lpOutput, 65536, &dwBytesTransferred, &eBufferType);
		if (wr->m_dwError != ERROR_SUCCESS)
		{
			return;
		}
		lpOutput[dwBytesTransferred] = 0;
		wr->m_receiver->dataReceived(lpOutput);
	}
}

void WinHTTPRequest::receiveWebSocketStream(IStreamReceiver *receiver)
{
	m_receiver = receiver;

	// Kick off a new thread to poll the connection and 
	// handle data when it arrives
	m_ReceiveThread = std::thread(receiveDataStatic, this);

	std::cout << "Finished thread creation method" << std::endl;
}