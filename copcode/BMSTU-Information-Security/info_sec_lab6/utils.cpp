#include "utils.h"
#include <math.h>
#include <fstream>

vector<string> split(string str, uint8_t delim)
{
	vector<string> strvec;
	string cur;
	for (uint8_t symb : str)
	{
		if (symb == delim)
		{
			if (cur.empty() == false)
			{
				strvec.push_back(cur);
				cur.clear();
			}
		}
		else
		{
			cur += symb;
		}
	}
	if (cur.empty() == false)
	{
		strvec.push_back(cur);
	}
	return strvec;
}

string uintToBin(uint8_t n)
{
	string result;
	do
	{
		string cur_char;
		cur_char = (n % 2 == 0) ? '0' : '1';
		result = cur_char + result;
		n = n / 2;
	} while (n > 0);

	string null_buffer;
	while ((result.size() + null_buffer.size()) < 8)
	{
		null_buffer.push_back('0');
	}

	result = null_buffer + result;


	return result;
}

string strToBin(string str)
{
	string res;
	for (uint8_t symb : str)
	{
		res += uintToBin(symb);
	}
	return res;
}


int strToFile(string filename, string str)
{
	ofstream fout;
	fout.open(filename, ios_base::trunc);
	if (fout.is_open())
	{
		fout << str;
		return 0;
	}
	return -1;
}
