#pragma once

#include <iostream>
#include <vector>
#include <functional>

#include "reflector.h"

class CryptEnigma
{
private:
	class Rotor;
public:
	CryptEnigma(size_t seed = 3);
	~CryptEnigma();
	std::string crypt(const std::string&);
	std::string encrypt(const std::string&);
private:
	void reset();
	uint8_t cryptOneChar(uint8_t c);
private:
	std::vector<Rotor> mRotors;
	Reflector mReflector;
	size_t mShiftedRotorIdx;
};

