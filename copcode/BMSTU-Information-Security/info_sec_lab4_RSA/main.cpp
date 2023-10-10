#include <iostream>
#include <inttypes.h>
#include <vector>
#include <limits>
#include <fstream>
#include "rsa.h"
#include "utils.h"
#include "debug.h"

using namespace std;

const string crypt_filename = "./rsa_crypt";
const string encrypt_filename = "./rsa_encrypt";

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
//	filename = "../tests/text";
	string init = readAllFromFile(filename);
	Debug::debug(filename);

	srand(time(nullptr));
	Debug::setType(Debug::Type::Debug, false);
	RSA rsa;
	cout << "Generating keys..." << endl;
	rsa.generateKeys(16);

	string crypt = rsa.crypt(init);
	string encrypt = rsa.encrypt(crypt);

	int err = strToFile(crypt_filename, crypt);
	if (err != 0)
	{
		cout << "Unable to create crypt file" << endl;
		return -1;
	}

	err = strToFile(encrypt_filename, encrypt);
	if (err != 0)
	{
		cout << "Unable to create encrypt file" << endl;
		return -1;
	}

	cout << "Done" << endl;
	return 0;
}
