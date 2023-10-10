#pragma once

#include <iostream>
#include <string>
#include "prime_number_generator.h"

using namespace std;

class RSA
{
public:
	string crypt(const string& str);
	string encrypt(const string& str);
	void generateKeys(const uint16_t min = 16);
private:
	int64_t findE(int64_t fi);
	int64_t findD(int64_t fi, int64_t E, int64_t N);
	int64_t EuclideanAlgorithm(int64_t a, int64_t b);
	vector<int64_t> extendedEuclideanAlgorithm(int64_t a, int64_t b);
	uint64_t moduloDegree(uint64_t base, uint64_t degree, uint64_t mod);
private:
	pair<uint64_t, uint64_t> mPublicKey;
	pair<uint64_t, uint64_t> mSecretKey;
	bool mKeysGenerated = false;
	uint64_t mN;
};

