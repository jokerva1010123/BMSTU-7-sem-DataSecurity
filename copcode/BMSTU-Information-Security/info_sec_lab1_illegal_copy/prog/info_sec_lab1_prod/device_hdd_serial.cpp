#include "device_hdd_serial.h"

DeviceHDDSerial::DeviceHDDSerial()
{
}

string DeviceHDDSerial::hDDSerial()
{
//	*is_ok = initSerial();
	return mHDDSerial;
}

bool DeviceHDDSerial::initSerial()
{
//	system("udevadm info --query=property --name=/dev/sda | grep SERIAL=");
	bool is_ok = true;

	char cmd[] = "udevadm info --query=property --name=/dev/sda | grep SERIAL=";
	char buf[BUFSIZ];
	FILE *ptr;

	if ((ptr = popen(cmd, "r")) != NULL)
	{
		while (fgets(buf, BUFSIZ, ptr) != NULL)
		{
			string tmp = buf;
			mHDDSerial = slpit(buf, "=").at(1);
		}
		pclose(ptr);
	}
	else
	{
		is_ok = false;
	}
	return is_ok;
}

ostream &DeviceHDDSerial::operator<<(ostream &os) const
{
	os << mHDDSerial;
	return os;
}

vector<string> DeviceHDDSerial::slpit(string str, string delim)
{
	vector<string> res;
	size_t tmp = str.find(delim);
	size_t cur_start = 0;
	while (tmp != string::npos)
	{
		string substring = str.substr(cur_start, tmp);
		substring = strip(substring);
		res.push_back(substring);
		cur_start = tmp + 1;
		tmp = str.find(delim, cur_start);
	}
	string s = str.substr(cur_start);
	s = strip(s);
	res.push_back(s);
	return res;
}

string DeviceHDDSerial::strip(string str)
{
	string res = str;
	size_t str_size = str.size();
	if (str.size() < 1)
	{
		return res;
	}
	if (str.find("\n") == str_size - 1)
	{
		res = str.substr(0, str_size - 1);
		str_size--;
	}
	return res;
}

void DeviceHDDSerial::setHDDSerial(const string &hDDSerial)
{
	mHDDSerial = hDDSerial;
}

bool DeviceHDDSerial::operator=(const DeviceHDDSerial &second)
{
	return (mHDDSerial == second.mHDDSerial);
}
