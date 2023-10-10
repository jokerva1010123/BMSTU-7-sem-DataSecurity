#pragma once

#include <inttypes.h>
#include <iostream>
#include <memory>
#include "mybitset.hpp"

template <uint64_t N>
class InitKey
{
public:
	InitKey();
	string toString();
	const MyBitset<N>& data();

private:
	MyBitset<N> mKey;
};

template<uint64_t N>
const MyBitset<N> &InitKey<N>::data()
{
	return mKey;
}


template<uint64_t N>
string InitKey<N>::toString()
{
	return mKey.to_string();
}

template <uint64_t N>
InitKey<N>::InitKey()
{
	srand(time (NULL));
	for (uint64_t i = 0; i < N; ++i)
	{
		uint8_t cur_char = 0 + rand() % 2;
		mKey[i] = cur_char;
	}
}

