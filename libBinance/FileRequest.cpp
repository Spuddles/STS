#include "FileRequest.h"
#include <fstream>
#include <streambuf>

FileRequest::FileRequest(const std::string &path) :
	m_Path(path)
{
}

FileRequest::~FileRequest()
{
}

std::string	FileRequest::ping()
{
	return "{}";
}

std::string	FileRequest::getTime()
{
	return "{\"serverTime\":1516135424195}"; // Jan 16 2017 20:44
}

std::string	FileRequest::getMarketDepth(const std::string & /*pair*/, unsigned int /*depth*/)
{
	return "";
}

std::string	FileRequest::getPairsPricesVolume()
{
	return "";
}

std::string	FileRequest::getPairsPrices()
{
	return "";
}

std::string	FileRequest::getProducts()
{
	return loadFile("allProducts.json");
}

std::string FileRequest::getHistoricPrices(const std::string & /*symbol*/, const std::string & /*interval*/, unsigned int /*amount*/)
{
	return loadFile("prices.json");
}

std::string FileRequest::getCurrentPrices()
{
	return loadFile("allPrices.json");
}

std::string FileRequest::getRequest(const std::string & /*url*/)
{
	return "{}";
}

std::string FileRequest::loadFile(const std::string &filename)
{
	std::string fullpath = m_Path + "\\" + filename;

	std::ifstream infile(fullpath);
	std::string str((std::istreambuf_iterator<char>(infile)),
		std::istreambuf_iterator<char>());

	infile.close();
	return str;
}
