#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

class DeviceHDDSerial
{
public:
	DeviceHDDSerial();
	string hDDSerial();
	void setHDDSerial(const string &hDDSerial);
	bool operator=(const DeviceHDDSerial& second);
	bool initSerial();
	ostream& operator<<(ostream& os) const;

private:
	vector<string> slpit(string str, string delim);
	string strip(string str);

	string mHDDSerial;
};

