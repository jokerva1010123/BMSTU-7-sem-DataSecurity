#include "installer.h"

int Installer::install(const string &filename)
{
	DeviceHDDSerial deviceHddSerial;
	bool is_ok = deviceHddSerial.initSerial();
	if (is_ok == false)
	{
		cerr << "Unable to read HDD serial numer" << endl;
		return -1;
	}
	string command_creatf = "sudo touch " + filename;
	system(command_creatf.c_str());
	ofstream fout;
	mLicenceFilename = filename;
	fout.open(mLicenceFilename, ios_base::out | ios_base::trunc);
	if (fout.is_open() == false)
	{
		cerr << "Unable to open file" << endl;
		return -2;
	}
	fout << deviceHddSerial.hDDSerial();
	fout.close();
//	string command = "sudo chmod 744 " + filename;
//	system(command.c_str());
	cout << "Installation complete" << endl;
	return 0;
}
