#include "prime_number_generator.h"
#include <algorithm>
#include <assert.h>
#include <debug.h>

int64_t PrimeNumberGenerator::generate(uint64_t min, uint64_t max)
{
	assert(min <= max);

	vector<uint64_t> prime_nums_vec = eratosthenesSieve(max);
	uint64_t prime_nums_vec_size = prime_nums_vec.size();

	vector<uint64_t>::iterator min_idx_iter = find_if(prime_nums_vec.begin(), prime_nums_vec.end(), [&](uint64_t num)
	{
		return num >= min;
	});
	uint64_t min_idx = distance(prime_nums_vec.begin(), min_idx_iter);
	if (min_idx >= prime_nums_vec_size)
	{
		return -1; // невозможно сгенерировать простое число в заданном диапазоне
	}
	uint64_t random_idx = min_idx + rand() % (prime_nums_vec_size - min_idx);
	Debug::debug("% prime_nums_vec.at(random_idx): %", __PRETTY_FUNCTION__, prime_nums_vec.at(random_idx));
	return prime_nums_vec.at(random_idx);
}

vector<uint64_t> PrimeNumberGenerator::eratosthenesSieve(uint64_t max)
{
	vector<uint64_t> prime_nums_vec;
	vector<int64_t> nums_vec;
	for (uint64_t i = 2; i <= max; ++i)
	{
		nums_vec.push_back(i);
	}

	for (vector<int64_t>::iterator iter = nums_vec.begin(); iter < nums_vec.end(); iter++)
	{
		int64_t cur_step = *iter;
		for (vector<int64_t>::iterator exclude_iter = iter + cur_step; exclude_iter <= nums_vec.end() && cur_step != -1; exclude_iter += cur_step)
		{
			*exclude_iter = -1;
		}
	}
	for (vector<int64_t>::iterator iter = nums_vec.begin(); iter < nums_vec.end(); iter++)
	{
		int64_t num = *iter;
		if ( num != -1 )
		{
			prime_nums_vec.push_back( static_cast<uint64_t>(num) );
		}
	}
	return prime_nums_vec;
}
