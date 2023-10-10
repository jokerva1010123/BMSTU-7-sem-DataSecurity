#include <assert.h>
#include "rotor.h"

CryptEnigma::Rotor::Rotor(uint8_t min_val, uint8_t max_val)
	: mCycle(0), mMinVal(min_val), mMaxVal(max_val), mOutSymbols(min_val, max_val), mInitialState(mOutSymbols)
{
	assert(min_val < max_val);
}

bool CryptEnigma::Rotor::shift()
{
	mCycle++;
	mCycle = mCycle == mOutSymbols.size() ? 0 : mCycle;

	mOutSymbols.crs();

	return mCycle == 0;
}


uint8_t CryptEnigma::Rotor::crypt(uint8_t c)
{
	uint64_t tmp = c - mMinVal;
	return mOutSymbols.at( tmp );
}

uint8_t CryptEnigma::Rotor::encrypt(uint8_t c)
{
	auto idx = mOutSymbols.indexOf(c);
	assert(idx >= 0);
	uint64_t tmp = idx + mMinVal;
	return static_cast<uint8_t>( tmp );
}

void CryptEnigma::Rotor::reset()
{
	mOutSymbols = mInitialState;
}
