#include "licence_checker.h"

int LicenceChecker::checkLicence()
{
	DeviceHDDSerial d;
	bool is_ok = d.initSerial();
	if (is_ok == false)
	{
		return 1;
	}
	ifstream fin;
	fin.open("licence.key");
	if (fin.is_open() == false)
	{
		return -1;
	}
	char buff[BUFSIZ];
	fin >> buff;
	string file_licence_str = buff;
	if (buff == d.hDDSerial())
	{
		return 0;
	}
	return -1;
}
