#pragma once

#include <iostream>
#include <fstream>
#include <string>

#include "device_hdd_serial.h"

class Installer
{
public:
	int install(const string& filename);
private:
	string mLicenceFilename;
};

