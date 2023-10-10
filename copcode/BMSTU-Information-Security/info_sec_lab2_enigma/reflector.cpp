#include <assert.h>
#include "reflector.h"

Reflector::Reflector(uint8_t min_val, uint8_t max_val)
	: mMinVal(min_val)
{
	assert(min_val <= max_val);
	init(min_val, max_val);
}

void Reflector::init(uint8_t min_val, uint8_t max_val)
{
	for (int64_t bytes = max_val; bytes >= min_val; --bytes)
	{
		mOutSymbols.push_back(bytes);
	}
}

unsigned char Reflector::crypt(uint8_t c)
{
	return mOutSymbols.at( c - mMinVal );
}
