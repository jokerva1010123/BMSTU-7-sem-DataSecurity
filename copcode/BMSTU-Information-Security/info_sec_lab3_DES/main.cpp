#include <iostream>
#include <fstream>
#include "mybitset.hpp"
#include "initkey.hpp"
#include "des.h"

using namespace std;

std::string readAllFromFile(const std::string& filename)
{
	std::string bytea;
	std::string buffer;
	std::fstream stream(filename);
	int ch;
	while ( (ch = stream.get()) != EOF)
	{
		bytea += ch;
	}
	return bytea;
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

int main()
{
	string filename;
	cout << "Input filename: ";
	cin >> filename;
	string init = readAllFromFile(filename);

	DES des;
	string crypt = des.crypt(init);
	string encrypt = des.encrypt(crypt);

	int err = strToFile("enigma_crypt", crypt);
	if (err != 0)
	{
		cout << "Unable to create crypt file" << endl;
		return -1;
	}

	err = strToFile("enigma_encrypt", encrypt);
	if (err != 0)
	{
		cout << "Unable to create encrypt file" << endl;
		return -1;
	}

	cout << "Done" << endl;

	return 0;
}
