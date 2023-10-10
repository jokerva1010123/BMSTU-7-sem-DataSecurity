#include <iostream>
#include <fstream>
#include <stdio.h>

#include "enigma.h"
#include "random_array.h"

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
//	FILE* f = fopen(filename.c_str(), "r");
//	if (f == NULL)
//	{
//		return bytea;
//	}
//	char val;
//	while((val = fscanf(f, "%c", &val) != EOF ))
//	{
//		bytea += static_cast<char>(val);
//	}

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

	CryptEnigma enigma(1);
	string init = readAllFromFile(filename);
	string crypt = enigma.crypt(init);
	int err = strToFile("enigma_crypt", crypt);
	if (err != 0)
	{
		cout << "Unable to create crypt file" << endl;
		return -1;
	}

	string encrypt = enigma.encrypt(crypt);
	err = strToFile("enigma_encrypt", encrypt);
	if (err != 0)
	{
		cout << "Unable to create crypt file" << endl;
		return -1;
	}

	return 0;
}
