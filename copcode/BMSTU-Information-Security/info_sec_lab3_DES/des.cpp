#include "des.h"
#include "mybitset.hpp"
#include "utils.h"
#include <math.h>
#include <vector>

DES::DES()
{
//	srand(time(NULL));
	keyGen(CRYPT_CYCLES);
}

string DES::crypt(string initial_str)
{
	string result;
	int parts = ceil(initial_str.size() / float(STRING_SIZE));
	for (int i = 0; i < parts; ++i)
	{
		string part = initial_str.substr(i * STRING_SIZE, STRING_SIZE);
		part.resize(STRING_SIZE);
		part = _strToBin(part);
		MyBitset<BITVEC_SIZE> bitvec(part);
		initialPermutation(bitvec); // начальная перестановка
		cryptCycles(bitvec); // шифр Фейстеля

		// конечная перестановка
		MyBitset<HALF_BITVEC_SIZE> L = bitsetLeft<BITVEC_SIZE>(bitvec);
		MyBitset<HALF_BITVEC_SIZE> R = bitsetRight<BITVEC_SIZE>(bitvec);
		MyBitset<BITVEC_SIZE> reversed_bitvec = merge<HALF_BITVEC_SIZE, HALF_BITVEC_SIZE>(R, L);
		finalPermutation(reversed_bitvec);
		result += reversed_bitvec.to_string();
	}
	mBinRes = result;
	result = _binStrToSymbols(result, STRING_SIZE);
	return result;
}

string DES::encrypt(string cipher)
{
	string result;
	int parts = ceil(cipher.size() / float(STRING_SIZE));
	for (int i = 0; i < parts; ++i)
	{
		string part = cipher.substr(i * STRING_SIZE, STRING_SIZE);
//		part.resize(STRING_SIZE);
		part = _strToBin(part);
		MyBitset<BITVEC_SIZE> bitvec(part);

		initialPermutation(bitvec); // начальная перестановка
		encryptCycles(bitvec); // обратный шифр Фейстеля
		// конечная перестановка
		MyBitset<HALF_BITVEC_SIZE> L = bitsetLeft<BITVEC_SIZE>(bitvec);
		MyBitset<HALF_BITVEC_SIZE> R = bitsetRight<BITVEC_SIZE>(bitvec);
		MyBitset<BITVEC_SIZE> reversed_bitvec = merge<HALF_BITVEC_SIZE, HALF_BITVEC_SIZE>(R, L);
		finalPermutation(reversed_bitvec);

		result += reversed_bitvec.to_string();
	}
	mBinRes = result;
	result = _binStrToSymbols(result, STRING_SIZE);
	return result;
}

string DES::binResult()
{
	return mBinRes;
}

void DES::finalPermutation(MyBitset<BITVEC_SIZE> &bitvec)
{
	const MyBitset<BITVEC_SIZE> copy(bitvec);
	for (uint64_t i = 0; i < IP_Back.size(); ++i)
	{
		bitvec[i] = copy[IP_Back[i] - 1];
	}
}

void DES::cryptCycles(MyBitset<BITVEC_SIZE> &bitvec)
{
	MyBitset<HALF_BITVEC_SIZE> cur_right = bitsetRight<BITVEC_SIZE>(bitvec);
	MyBitset<HALF_BITVEC_SIZE> cur_left = bitsetLeft<BITVEC_SIZE>(bitvec);
	for (uint8_t i = 1; i <= CRYPT_CYCLES; ++i)
	{
		MyBitset<HALF_BITVEC_SIZE> new_left = cur_right;
		MyBitset<HALF_BITVEC_SIZE> f = F(cur_right, mKeys[i-1]);
		MyBitset<HALF_BITVEC_SIZE> new_right = cur_left ^ f;
		cur_left = new_left;
		cur_right = new_right;
	}

	bitvec = merge<HALF_BITVEC_SIZE, HALF_BITVEC_SIZE>(cur_right, cur_left);
}

void DES::encryptCycles(MyBitset<BITVEC_SIZE> &bitvec)
{
	MyBitset<HALF_BITVEC_SIZE> cur_right = bitsetRight<BITVEC_SIZE>(bitvec);
	MyBitset<HALF_BITVEC_SIZE> cur_left = bitsetLeft<BITVEC_SIZE>(bitvec);
	for (int8_t i = CRYPT_CYCLES - 1; i >= 0; --i)
	{
		MyBitset<HALF_BITVEC_SIZE> prev_right = cur_left;
		MyBitset<HALF_BITVEC_SIZE> f = F(cur_left, mKeys[i]);
		MyBitset<HALF_BITVEC_SIZE> prev_left = cur_right ^ f;
		cur_left = prev_left;
		cur_right = prev_right;
	}

	bitvec = merge<HALF_BITVEC_SIZE, HALF_BITVEC_SIZE>(cur_right, cur_left);
}

void DES::initialPermutation(MyBitset<BITVEC_SIZE>& bitvec)
{
	const MyBitset<BITVEC_SIZE> copy(bitvec);
	for (uint64_t i = 0; i < IP.size(); ++i)
	{
		bitvec[i] = copy[IP[i] - 1];
	}
}

MyBitset<INIT_KEY_SIZE> DES::generateInitRoundKey(const MyBitset<INIT_KEY_SIZE> &init_key)
{
	MyBitset<BITVEC_SIZE> expanded_key;
	uint16_t units_count = 0;

	for (uint64_t i = 0, j = 0; i < INIT_KEY_SIZE; ++i, ++j)
	{
		if (i % 7 != 0 || i == 0)
		{
			units_count = (init_key[i] == 1) ? (units_count + 1) : (units_count);
			expanded_key[j] = init_key[i];
		}
		else
		{
			expanded_key[j] = (units_count % 2 == 0) ? (1) : (0);
			++j;
			units_count = 0;
			units_count = (init_key[i] == 1) ? (units_count + 1) : (units_count);
			expanded_key[j] = init_key[i];
		}
	}

	MyBitset<INIT_KEY_SIZE> shuffle_expanded_key;

	for (uint64_t i = 0; i < INIT_KEY_SIZE; ++i)
	{
		shuffle_expanded_key[i] = expanded_key[Round_IP[i] - 1];
	}
	return shuffle_expanded_key;
}

void DES::keyGen(uint64_t count)
{
	MyBitset<INIT_KEY_SIZE> init_round_key = generateInitRoundKey(mKey.data());
	const uint64_t half_key = INIT_KEY_SIZE / 2;

	MyBitset<half_key> Ci = bitsetLeft<INIT_KEY_SIZE>(init_round_key);
	MyBitset<half_key> Di = bitsetRight<INIT_KEY_SIZE>(init_round_key);

	for (uint64_t i = 0; i < count; ++i)
	{
		uint16_t shift = Shift[i];
		Ci.cyclicShiftLeft(shift);
		Di.cyclicShiftLeft(shift);

		MyBitset<INIT_KEY_SIZE> merged_key = merge<half_key, half_key>(Ci, Di);
		MyBitset<EXPANSION> final_key = compressKey(merged_key);
		mKeys.push_back(final_key);
	}
}

MyBitset<EXPANSION> DES::compressKey(const MyBitset<INIT_KEY_SIZE> &key)
{
	MyBitset<EXPANSION> res;
	for (uint64_t i = 0; i < EXPANSION; ++i)
	{
		res[i] = key[Contraction_Permutation[i] - 1];
	}
	return res;
}

MyBitset<HALF_BITVEC_SIZE> DES::F(const MyBitset<HALF_BITVEC_SIZE> &cur_rigth, const MyBitset<EXPANSION> &cur_key)
{
	MyBitset<EXPANSION> expanded_rigth = expand(cur_rigth);
	expanded_rigth ^= cur_key;

	vector<MyBitset<BLOCK_SIZE>> blocks = divide<EXPANSION, BLOCK_SIZE>(expanded_rigth);

	MyBitset<SHORT_FULL_BLOCK> full_short_block;

	for (uint16_t i = 0, j = 0, n = blocks.size(); i < n; ++i)
	{
		uint16_t a = getA(blocks[i]);
		uint16_t b = getB(blocks[i]);
		uint8_t num = S[i][a][b];
		string bin_num = _charToBin(num, 4);
		MyBitset<SHORT_BLOCK_SIZE> vec_new(bin_num);
		insert<SHORT_FULL_BLOCK, SHORT_BLOCK_SIZE>(full_short_block, vec_new, j);
		j += SHORT_BLOCK_SIZE;
	}
	permutationP(full_short_block);
	return full_short_block;
}

void DES::permutationP(MyBitset<SHORT_FULL_BLOCK>& vec)
{
	const MyBitset<SHORT_FULL_BLOCK> copy(vec);
	for (uint16_t i = 0; i < SHORT_BLOCK_SIZE; ++i)
	{
		vec[i] = copy[Permutation_P[i] - 1];
	}
}

uint16_t DES::getA(const MyBitset<BLOCK_SIZE>& vec)
{
	string a;
	a = (vec[0] == 1) ? '1' : '0';
	a += (vec[BLOCK_SIZE-1] == 1) ? '1' : '0';
	uint16_t res = _binStrToUlong(a);
	return res;
}

uint16_t DES::getB(const MyBitset<BLOCK_SIZE> &vec)
{
	string b;
	for (uint16_t j = 1; j < BLOCK_SIZE-1; ++j)
	{
		b += (vec[j] == 1) ? '1' : '0';
	}
	uint16_t res = _binStrToUlong(b);
	return res;
}

MyBitset<EXPANSION> DES::expand(const MyBitset<HALF_BITVEC_SIZE> &vec)
{
	MyBitset<EXPANSION> res;
	for (uint64_t i = 0; i < EXPANSION; ++i)
	{
		res[i] = vec[Expanding_Permutation[i] - 1];
	}
	return res;
}

// 00000000 00011111 00011101 00010000
// 00000000 00000000 11111110 10001111 10101000 10100000
