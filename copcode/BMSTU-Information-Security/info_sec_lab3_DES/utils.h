#pragma once

#include <string>
#include <iostream>
#include <inttypes.h>

using namespace std;

string _charToBin(const uint8_t &symb, uint16_t size);
string _strToBin(const string& str);
uint8_t _binStrToUlong(const string& str);
string _binStrToSymbols(const string& bitstr, uint64_t symb_bits_size);
