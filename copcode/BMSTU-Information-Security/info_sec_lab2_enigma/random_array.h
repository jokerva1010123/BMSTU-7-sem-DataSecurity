#pragma once

#include <iostream>
#include <vector>

class ShuffleCharArray final
{
public:
	ShuffleCharArray(uint8_t min_val, uint8_t max_val);
	bool contains(uint8_t elem);
	size_t size();
	unsigned char at(uint8_t i) const;
	void pushBack(uint8_t elem);
	const u_int8_t *data() const;
	int16_t indexOf(uint8_t);
	void crs();
private:
	void init(uint8_t min_val, uint8_t max_val);

private:
	std::vector<uint8_t> mShaffleCharArr;
};

