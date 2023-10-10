#pragma once

#include <inttypes.h>
#include <vector>
#include <iostream>
#include <limits>

using namespace std;

class PrimeNumberGenerator
{
public:
	static int64_t generate(uint64_t min = 2, uint64_t max = UINT16_MAX);
	static vector<uint64_t> eratosthenesSieve(uint64_t max);
};

