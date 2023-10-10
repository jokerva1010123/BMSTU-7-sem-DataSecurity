#include "utils.h"
#include <math.h>

string _uintToBin(uint64_t n)
{
	string result;
	do
	{
		string cur_char;
		cur_char = (n % 2 == 0) ? '0' : '1';
		result = cur_char + result;
		n = n / 2;
	} while (n > 0);
	return result;
}

uint64_t _binStrToUlong(const string& str)
{
	uint64_t res = 0;
	for (int64_t i = str.size() - 1, j = 0; i >= 0; --i, ++j)
	{
//		uint32_t factor = strtol(str.data() + j, nullptr, 10);
		uint64_t factor = (str[j] == '1') ? 1 : 0;
		res += factor * pow(2, i);
	}
	return res;
}

string supplement(string str, uint64_t size)
{
	uint64_t sstr = str.size();
	if (sstr < size)
	{
		int diff = size - sstr;
		for (int64_t i = 0; i < diff; ++i)
		{
			str = "0" + str;
		}
	}
	return str;
}

string _binStrToSymbols(const string& bitstr, uint64_t symb_bits_size)
{
	string res;
	uint64_t n = bitstr.size() / (sizeof (char) * 8);
	for (uint64_t i = 0; i < n; ++i)
	{
		string substr = bitstr.substr(i * symb_bits_size, symb_bits_size);
		uint8_t ch = _binStrToUlong(substr);
		res += ch;
	}
	return res;
}
