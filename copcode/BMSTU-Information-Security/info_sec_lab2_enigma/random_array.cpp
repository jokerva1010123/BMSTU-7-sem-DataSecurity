#include "random_array.h"
#include <limits>
#include <random>
#include <algorithm>
#include <utility>

//constexpr size_t MaxSizeByte = std::numeric_limits<unsigned char>::max() + 1;

ShuffleCharArray::ShuffleCharArray(uint8_t min_val, uint8_t max_val)
{
	init(min_val, max_val);
}

bool ShuffleCharArray::contains(unsigned char elem)
{
	auto iter = std::find(mShaffleCharArr.begin(), mShaffleCharArr.end(), elem);
	return iter != mShaffleCharArr.end();
}

size_t ShuffleCharArray::size()
{
	return mShaffleCharArr.size();
}

unsigned char ShuffleCharArray::at(uint8_t i) const
{
	return mShaffleCharArr.at(i);
}

void ShuffleCharArray::pushBack(uint8_t elem)
{
	mShaffleCharArr.push_back(elem);
}

const u_int8_t *ShuffleCharArray::data() const
{
	return mShaffleCharArr.data();
}

int16_t ShuffleCharArray::indexOf(uint8_t elem)
{
	int16_t out_val = -1;
	auto iter = std::find(mShaffleCharArr.begin(), mShaffleCharArr.end(), elem);

	if (iter != mShaffleCharArr.end())
	{
		out_val = std::distance(mShaffleCharArr.begin(), iter);
	}

	return out_val;
}

void ShuffleCharArray::crs()
{
	uint8_t val = mShaffleCharArr.at(mShaffleCharArr.size() - 1);
	mShaffleCharArr.erase(mShaffleCharArr.end() - 1);
	mShaffleCharArr.insert(mShaffleCharArr.begin(), val);
}

void ShuffleCharArray::init(uint8_t _min, uint8_t _max)
{
	mShaffleCharArr.reserve(_max - _min);
	for (int64_t byte = _min; byte <= _max; ++byte)
	{
		mShaffleCharArr.push_back(byte);
	}

	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(mShaffleCharArr.begin(), mShaffleCharArr.end(), g);
}
