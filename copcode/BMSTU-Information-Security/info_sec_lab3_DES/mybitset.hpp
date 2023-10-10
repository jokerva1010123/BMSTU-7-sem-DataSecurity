#pragma once

#include <bitset>
#include <inttypes.h>
#include <iostream>
#include <vector>
#include <math.h>

#include "utils.h"

using namespace std;

template<uint64_t N>
class MyBitset : public bitset<N>
{
public:
	MyBitset();
	MyBitset(const string& str);
	typename bitset<N>::reference operator[](uint64_t pos);
	bool operator[](uint64_t pos) const;
	void cyclicShiftLeft(uint64_t shift);
	MyBitset& operator^(const MyBitset<N>& s);
};

template<uint64_t N, uint64_t M>
void insert(MyBitset<N>& to, const MyBitset<M>& from, uint64_t pos)
{
	if(pos >= N || pos + M - 1 >= N)
	{
		throw out_of_range("Bitset index out of range");
	}

	for (uint64_t i = pos, j = 0; j < M; ++i,++j)
	{
		to[i] = from[j];
	}
}

template<uint64_t N>
MyBitset<N> &MyBitset<N>::operator^(const MyBitset<N> &s)
{
	for (uint64_t i = 0; i < N; ++i)
	{
		this->operator[](i) = this->operator[](i) ^ s[i];
	}
	return *this;
}

template <uint64_t N, uint64_t M>
vector<MyBitset<M>> divide(const MyBitset<N>& vec)
{
	vector<MyBitset<M>> res;
	uint64_t blocks = ceil(N / M);
	res.resize(blocks);
	for (uint64_t i = 0; i < blocks; ++i)
	{
		MyBitset<M> block;
		for (uint64_t j = i * M, c = 0; j < (i + 1) * M; ++j, c++)
		{
			block[c] = vec[j];
		}
		res[i] = block;
	}
	return res;
}

template <int N>
MyBitset<N / 2> bitsetRight(const MyBitset<N>& bitvec)
{
	MyBitset<N / 2> res;
	for (uint64_t i = N / 2, j = 0; i < N; ++i, ++j)
	{
		res[j] = bitvec[i];
	}
	return res;
}

template <int N>
MyBitset<N / 2> bitsetLeft(const MyBitset<N>& bitvec)
{
	MyBitset<N / 2> res;
	for (uint64_t i = 0; i < N / 2; ++i)
	{
		res[i] = bitvec[i];
	}
	return res;
}

template <int N, int M>
MyBitset<N + M> merge(const MyBitset<N>& f, const MyBitset<M>& s)
{
	MyBitset<N + M> res;

	for (uint64_t i = 0; i < N; ++i)
	{
		res[i] = f[i];
	}
	uint64_t count = N;
	for (uint64_t i = 0; i < M; ++i)
	{
		res[count] = s[i];
		count++;
	}

	return res;
}

template<uint64_t N>
void MyBitset<N>::cyclicShiftLeft(uint64_t shift)
{
	MyBitset<N> vec_new;
	uint64_t i = 0 + shift % N;

	for (uint64_t c = 0; c < N; ++c)
	{
		vec_new[c] = this->operator[](i);
		i = (i + 1) % N;
	}

	this->reset();

	for (uint64_t i = 0; i < N; ++i)
	{
		this->operator[](i) = vec_new[i];
	}
}


template<uint64_t N>
MyBitset<N>::MyBitset(const string& str)
	: bitset<N>(str)
{

}

template<uint64_t N>
MyBitset<N>::MyBitset()
	: bitset<N>()
{

}

template<uint64_t N>
typename bitset<N>::reference MyBitset<N>::operator[](uint64_t pos)
{
	pos = N - pos - 1;
	if (pos >= N)
	{
		throw out_of_range("Bitset index out of range");
	}
	return typename bitset<N>::reference(*this, pos);
//	return mData[pos];
}

template<uint64_t N>
bool MyBitset<N>::operator[](uint64_t pos) const
{
	pos = N - pos - 1;
	if (pos >= N)
	{
		throw out_of_range("Bitset index out of range");
	}
	return bitset<N>::_Unchecked_test(pos);
}

