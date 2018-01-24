#include "stdafx.h"
#include "WebSocketRequest.h"


WebSocketRequest::WebSocketRequest()
{
}


WebSocketRequest::~WebSocketRequest()
{
}


std::string WebSocketRequest::ping()
{
	return "";
}

std::string WebSocketRequest::getTime()
{
	return "";
}

std::string WebSocketRequest::getMarketDepth(std::string pair, unsigned int depth)
{
	return "";
}

std::string WebSocketRequest::getPairsPricesVolume()
{
	return "";
}

std::string WebSocketRequest::getPairsPrices()
{
	return "";
}

std::string WebSocketRequest::getProducts()
{
	return "";
}

std::string WebSocketRequest::getPrices(std::string symbol, std::string interval, unsigned int amount)
{
	return "";
}
