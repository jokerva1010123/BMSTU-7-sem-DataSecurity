#pragma once

#include <iostream>
#include <vector>

#include "random_array.h"

class Reflector
{
public:
	Reflector(uint8_t min_val, uint8_t max_val);
	unsigned char crypt(uint8_t c);
private:
	void init(uint8_t min_val, uint8_t max_val);
private:
	uint8_t mMinVal;
	std::vector<uint8_t> mOutSymbols;
};

