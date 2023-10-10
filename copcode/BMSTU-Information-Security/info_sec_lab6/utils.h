#pragma once

#include <string>
#include <vector>

using namespace std;

vector<string> split(string str, uint8_t delim);
string uintToBin(uint8_t n);
string strToBin(string str);
int strToFile(string filename, string str);
