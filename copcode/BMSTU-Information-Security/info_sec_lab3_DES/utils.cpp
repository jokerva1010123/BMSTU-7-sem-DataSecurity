#include "utils.h"
#include <math.h>

uint8_t _binStrToUlong(const string& str)
{
	uint32_t res = 0;
	for (int16_t i = str.size() - 1, j = 0; i >= 0; --i, ++j)
	{
//		uint32_t factor = strtol(str.data() + j, nullptr, 10);
		uint32_t factor = (str[j] == '1') ? 1 : 0;
		res += factor * pow(2, i);
	}
	return res;
}

string _binStrToSymbols(const string& bitstr, uint64_t symb_bits_size)
{
	string res;
	uint16_t n = bitstr.size() / (sizeof (char) * 8);
	for (uint16_t i = 0; i < n; ++i)
	{
		string substr = bitstr.substr(i * symb_bits_size, symb_bits_size);
		uint8_t ch = _binStrToUlong(substr);
		res += ch;
	}
	return res;
}

string _charToBin(const uint8_t &symb, uint16_t size)
{
	string res;
	res.resize(size);
	for (int i = size - 1, j = 0; i >= 0; --i, j++)
	{
		uint cur_digit = (symb & (1 << i)) ? '1' : '0';
		res[j] = cur_digit;
	}
	return res;
}

string _strToBin(const string& str)
{
	string bin_str;
	for (uint8_t symb : str)
	{
		string bin_char = _charToBin(symb, 8);
		bin_str += bin_char;
	}
	return bin_str;
}
