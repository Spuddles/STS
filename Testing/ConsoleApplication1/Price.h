#pragma once
#include <stdint.h>

class Price
{
public:
				Price();
	virtual		~Price();

private:
	uint64_t	openTime;
	double		open;
	double		high;
	double		low;
	double		close;
	double		volume;
	uint64_t	closeTime;
	uint64_t	trades;
};

