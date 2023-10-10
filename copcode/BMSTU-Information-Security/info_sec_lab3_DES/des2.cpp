#include "des2.h"
#include <array>
#include <math.h>
#include <bitset>
#include <iostream>
#include <DES_params.h>

void initialPermutation(bitset<BITVEC_SIZE>& bitvec);
void finalPermutation(bitset<BITVEC_SIZE>& bitvec);
bitset<HALF_BITVEC_SIZE> F(const bitset<HALF_BITVEC_SIZE>& cur_rigth, const bitset<EXPANSION>& cur_key);
void cryptCycles(bitset<BITVEC_SIZE>& bitvec);
bitset<BITVEC_SIZE> strToBin(const string& str); // +
string charToBin(const uint8_t &symb); // +
uint8_t binStrToUlong(const string& str); // +
bitset<INIT_KEY_SIZE> generateInitKey(uint64_t size);
bitset<INIT_KEY_SIZE> generateInitRoundKey(const bitset<INIT_KEY_SIZE>& init_key);
vector<bitset<EXPANSION>> generateKeys(uint64_t count);
bitset<EXPANSION> keyGen(const bitset<INIT_KEY_SIZE>& k);
bitset<EXPANSION> expand(const bitset<HALF_BITVEC_SIZE>& vec);
vector<bitset<BLOCK_SIZE>> divide(const bitset<EXPANSION>& vec);
bitset<SHORT_FULL_BLOCK> permutationP(const bitset<SHORT_FULL_BLOCK>& vec);
uint16_t getA(const bitset<BLOCK_SIZE>& vec);
uint16_t getB(const bitset<BLOCK_SIZE>& vec);
string bitsetToString(const string& bitstr); // +

string charToBin(const uint8_t &symb)
{
	string res;
	for (int i = 7; i >= 0; --i)
	{
		uint cur_digit = (symb & (1 << i)) ? '1' : '0';
		res += cur_digit;
	}
	return res;
}

string bitsetToString(const string& bitstr)
{
	string res;
	uint16_t n = bitstr.size() / (sizeof (char) * 8);
	for (uint16_t i = 0; i < n; ++i)
	{
		string substr = bitstr.substr(i * STRING_SIZE, STRING_SIZE);
		uint8_t ch = binStrToUlong(substr);
		res += ch;
	}
	return res;
}

template <int N, int M>
bitset<N + M> merge(const bitset<N>& f, const bitset<M>& s) // +
{
	bitset<N + M> res;

	for (uint64_t i = 0; i < N; ++i)
	{
		res[i] = f[i];
	}
	uint64_t count = N;
	for (uint64_t i = 0; i < M; ++i)
	{
		res[count] = s[i];
	}

	return res;
}

template <int N>
void cyclicShiftLeft(bitset<N>& bitvec, uint64_t shift) // +
{
	bitset<N> vec_new;
	uint64_t i = 0 + shift % N;

	for (uint64_t c = 0; c < N; ++c)
	{
		vec_new[c] = bitvec[i];
		i = (i + 1) % N;
	}

	bitvec = vec_new;
}

template <int N>
bitset<N / 2> bitsetLeft(const bitset<N>& bitvec) // +
{
	bitset<N / 2> res;
	for (uint64_t i = 0; i < N / 2; ++i)
	{
		res[i] = bitvec[i];
	}
	return res;
}

template <int N>
bitset<N / 2> bitsetRight(const bitset<N>& bitvec) // +
{
	bitset<N / 2> res;
	for (uint64_t i = N / 2, j = 0; i < N; ++i, ++j)
	{
		res[j] = bitvec[i];
	}
	return res;
}

string DES2::crypt(string initial_str)
{
	string result;
	int parts = ceil(initial_str.size() / float(STRING_SIZE));
	for (int i = 0; i < parts; ++i)
	{
		string part = initial_str.substr(i * STRING_SIZE, STRING_SIZE);
		if (part.size() != STRING_SIZE)
		{
			part.resize(STRING_SIZE);
		}
		bitset<BITVEC_SIZE> bitvec = strToBin(part);
		initialPermutation(bitvec);
		cryptCycles(bitvec);
		bitset<HALF_BITVEC_SIZE> L = bitsetRight<BITVEC_SIZE>(bitvec);
		bitset<HALF_BITVEC_SIZE> R = bitsetLeft<BITVEC_SIZE>(bitvec);
		bitset<BITVEC_SIZE> reversed_bitvec = merge<HALF_BITVEC_SIZE, HALF_BITVEC_SIZE>(R, L);
		finalPermutation(reversed_bitvec);
//		cout << reversed_bitvec << endl << endl;
		result += reversed_bitvec.to_string();
	}
	result = bitsetToString(result);
	return result;
}

void cryptCycles(bitset<BITVEC_SIZE> &bitvec)
{
	vector<bitset<EXPANSION>> keys = generateKeys(CRYPT_CYCLES);

	bitset<HALF_BITVEC_SIZE> cur_right = bitsetLeft<BITVEC_SIZE>(bitvec);
	bitset<HALF_BITVEC_SIZE> cur_left = bitsetRight<BITVEC_SIZE>(bitvec);

	for (uint8_t i = 1; i <= CRYPT_CYCLES; ++i)
	{
		bitset<HALF_BITVEC_SIZE> new_left = cur_right;
		bitset<HALF_BITVEC_SIZE> new_right = cur_left ^ F(cur_right, keys[i]);

		cur_left = new_left;
		cur_right = new_right;
	}
}

vector<bitset<EXPANSION>> generateKeys(uint64_t count)
{
	vector<bitset<EXPANSION>> keys(count);
	bitset<INIT_KEY_SIZE> init_bin_key = generateInitKey(INIT_KEY_SIZE);
	bitset<INIT_KEY_SIZE> init_round_key = generateInitRoundKey(init_bin_key);
	const uint64_t init_keys_size_half = INIT_KEY_SIZE / 2;

	for (uint64_t i = 0; i < count; ++i)
	{
		bitset<init_keys_size_half> Ci = bitsetLeft<INIT_KEY_SIZE>(init_round_key);
		bitset<init_keys_size_half> Di = bitsetRight<INIT_KEY_SIZE>(init_round_key);
		cyclicShiftLeft<init_keys_size_half>(Ci, Shift[i]);
		cyclicShiftLeft<init_keys_size_half>(Di, Shift[i]);

		bitset<INIT_KEY_SIZE> merged_key = merge<init_keys_size_half, init_keys_size_half>(Ci, Di);

		bitset<EXPANSION> key = keyGen(merged_key);
		keys[i] = key;
	}
	return keys;
}

bitset<EXPANSION> keyGen(const bitset<INIT_KEY_SIZE>& k)
{
	bitset<EXPANSION> res;
	for (uint64_t i = 0; i < EXPANSION; ++i)
	{
		res[i] = k[Contraction_Permutation[i]];
	}
	return res;
}

bitset<INIT_KEY_SIZE> generateInitRoundKey(const bitset<INIT_KEY_SIZE>& init_key)
{
	bitset<BITVEC_SIZE> expanded_key;
	uint16_t units_count = 0;

	for (uint64_t i = 0, j = 0; j < BITVEC_SIZE; ++j)
	{
		if (j % 8 != 0 || j == 0)
		{
			units_count = (init_key[i] == 1) ? (units_count + 1) : (units_count);
			expanded_key[j] = init_key[i];
			++i;
		}
		else
		{
			expanded_key[j] = (units_count % 2 == 0) ? (1) : (0);
		}
	}

	bitset<INIT_KEY_SIZE> shuffle_expanded_key;

	for (uint64_t i = 0; i < INIT_KEY_SIZE; ++i)
	{
		shuffle_expanded_key[i] = expanded_key[Round_IP[i]];
	}
	return shuffle_expanded_key;
}

bitset<EXPANSION> expand(const bitset<HALF_BITVEC_SIZE>& vec)
{
	bitset<EXPANSION> res;
	for (uint64_t i = 0; i < EXPANSION; ++i)
	{
		res[i] = vec[Expanding_Permutation[i]];
	}
	return res;
}

bitset<INIT_KEY_SIZE> generateInitKey(uint64_t size)
{
	srand(time (NULL));
	bitset<INIT_KEY_SIZE> res;

	for (uint64_t i = 0; i < size; ++i)
	{
		uint8_t cur_char = MIN_KEY_CHAR + rand() % RAND_SIZE;
		res[i] = cur_char;
	}
	return res;
//	string key;

//	for (uint64_t i = 0; i < size; ++i)
//	{
//		uint8_t cur_char = MIN_KEY_CHAR + rand() % RAND_SIZE; // char from 0 to 255
//		key += cur_char;
//	}

//	return key;
}

bitset<BITVEC_SIZE> strToBin(const string& str)
{
	string bin_str;
	for (uint8_t symb : str)
	{
		string bin_char = charToBin(symb);
		bin_str += bin_char;
	}
	bitset<BITVEC_SIZE> res(bin_str);
	return res;
}

bitset<HALF_BITVEC_SIZE> F(const bitset<HALF_BITVEC_SIZE>& cur_rigth, const bitset<EXPANSION> &cur_key)
{
	bitset<EXPANSION> expanded_rigth = expand(cur_rigth);

	expanded_rigth ^= cur_key;
	vector<bitset<BLOCK_SIZE>> blocks = divide(expanded_rigth);

	bitset<SHORT_FULL_BLOCK> full_short_block;

	for (uint16_t i = 0, j = 0, n = blocks.size(); i < n; ++i)
	{
		uint16_t a = getA(blocks[i]);
		uint16_t b = getB(blocks[i]);
		uint8_t num = S[i][a][b];
		string bin_num = charToBin(num);
		bitset<SHORT_BLOCK_SIZE> vec_new(bin_num);
		for (uint16_t k = 0; k < SHORT_BLOCK_SIZE; ++k, ++j)
		{
			full_short_block[j] = vec_new[k];
		}
		j++;
	}
	bitset<SHORT_FULL_BLOCK> res = permutationP(full_short_block);
	return res;
}

bitset<SHORT_FULL_BLOCK> permutationP(const bitset<SHORT_FULL_BLOCK>& vec)
{
	bitset<SHORT_FULL_BLOCK> res;
	for (uint16_t i = 0; i < SHORT_BLOCK_SIZE; ++i)
	{
		res[i] = vec[Permutation_P[i]];
	}
	return res;
}

uint8_t binStrToUlong(const string& str)
{
	uint32_t res = 0;
	for (int16_t i = str.size() - 1, j = 0; i >= 0; --i, ++j)
	{
//		uint32_t factor = strtol(str.data() + j, nullptr, 10);
		uint32_t factor = (str[j] == '1') ? 1 : 0;
		res += factor * pow(2, i);
	}
	return res;
}

uint16_t getA(const bitset<BLOCK_SIZE>& vec)
{
	string a;
	a = (vec[0] == 1) ? '1' : '0';
	a += (vec[BLOCK_SIZE-1] == 1) ? '1' : '0';
	uint16_t res = binStrToUlong(a);
	return res;
}

uint16_t getB(const bitset<BLOCK_SIZE>& vec)
{
	string b;
	for (uint16_t j = 1; j < BLOCK_SIZE-1; ++j)
	{
		b += (vec[j] == 1) ? '1' : '0';
	}
	uint16_t res = binStrToUlong(b);
	return res;
}

vector<bitset<BLOCK_SIZE>> divide(const bitset<EXPANSION>& vec)
{
	vector<bitset<BLOCK_SIZE>> res;
	res.resize(BLOCKS);
	for (uint64_t i = 0; i < BLOCKS; ++i)
	{
		bitset<BLOCK_SIZE> block;
		for (uint64_t j = i * BLOCK_SIZE, c = 0; j < (i + 1) * BLOCK_SIZE; ++j, c++)
		{
			block[c] = vec[j];
		}
		res[i] = block;
	}
	return res;
}

void initialPermutation(bitset<BITVEC_SIZE>& bitvec)
{
	bitset<BITVEC_SIZE> result;
	for (uint64_t i = 0; i < IP.size(); ++i)
	{
		result[i] = bitvec[IP[i]];
	}
	bitvec = result;
}

void finalPermutation(bitset<BITVEC_SIZE> &bitvec)
{
	bitset<BITVEC_SIZE> result;
	for (uint64_t i = 0; i < IP_Back.size(); ++i)
	{
		result[i] = bitvec[IP_Back[i]];
	}
	bitvec = result;
}
