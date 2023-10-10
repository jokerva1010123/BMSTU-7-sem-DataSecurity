#pragma once

#include <string>
#include <iostream>
#include <vector>

#include "initkey.hpp"
#include "DES_params.h"

using namespace std;

class DES
{
public:
	DES();
	string crypt(string initial_str);
	string encrypt(string cipher);
	string binResult();
private:
	void cryptCycles(MyBitset<BITVEC_SIZE> &bitvec);
	void encryptCycles(MyBitset<BITVEC_SIZE> &bitvec);
	void initialPermutation(MyBitset<BITVEC_SIZE>& bitvec);
	MyBitset<INIT_KEY_SIZE> generateInitRoundKey(const MyBitset<INIT_KEY_SIZE>& init_key);
	void keyGen(uint64_t count);
	MyBitset<EXPANSION> compressKey(const MyBitset<INIT_KEY_SIZE>& key);
	MyBitset<HALF_BITVEC_SIZE> F(const MyBitset<HALF_BITVEC_SIZE>& cur_rigth, const MyBitset<EXPANSION> &cur_key);
	MyBitset<EXPANSION> expand(const MyBitset<HALF_BITVEC_SIZE>& vec);
	uint16_t getA(const MyBitset<BLOCK_SIZE> &vec);
	uint16_t getB(const MyBitset<BLOCK_SIZE>& vec);
	void permutationP(MyBitset<SHORT_FULL_BLOCK>& vec);
	void finalPermutation(MyBitset<BITVEC_SIZE> &bitvec);
private:
	InitKey<INIT_KEY_SIZE> mKey;
	vector<MyBitset<EXPANSION>> mKeys;
	string mBinRes;
};

