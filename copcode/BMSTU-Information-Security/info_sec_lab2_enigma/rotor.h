#pragma once

#include <iostream>
#include <array>
#include "enigma.h"
#include "random_array.h"

class CryptEnigma::Rotor final
{
public:
	Rotor(uint8_t min_val, uint8_t max_val);
	bool shift();
	uint8_t crypt(uint8_t c);
	uint8_t encrypt(uint8_t c);
	void reset();
private:
	size_t mCycle;
	uint8_t mMinVal;
	uint8_t mMaxVal;
	ShuffleCharArray mOutSymbols;
	ShuffleCharArray mInitialState;
};

