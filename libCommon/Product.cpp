#include "Product.h"

using namespace STS;

Product::Product()
{
}

Product::~Product()
{
}

std::string	Product::handleString(const unsigned char* text)
{
	if (text)
	{
		return std::string((const char*)text);
	}
	return "";
}
