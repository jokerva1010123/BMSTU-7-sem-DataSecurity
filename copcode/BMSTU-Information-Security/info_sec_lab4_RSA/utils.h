#pragma once

#include <iostream>
#include <inttypes.h>
#include <string>

using namespace std;

string _uintToBin(uint64_t n);
string _binStrToSymbols(const string& bitstr, uint64_t symb_bits_size);
string supplement(string str, uint64_t size);
uint64_t _binStrToUlong(const string& str);

