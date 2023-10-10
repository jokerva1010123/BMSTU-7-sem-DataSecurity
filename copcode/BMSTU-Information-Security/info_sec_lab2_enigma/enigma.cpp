#include <assert.h>
#include "enigma.h"
#include "rotor.h"

constexpr uint8_t MinChar_Value = 0;
constexpr uint8_t MaxChar_Value = 255;

CryptEnigma::CryptEnigma(size_t seed)
	: mReflector(MinChar_Value, MaxChar_Value), mShiftedRotorIdx(0)
{
	assert(seed > 0);
	for (size_t i = 0; i < seed; ++i)
	{
		mRotors.push_back(Rotor(MinChar_Value, MaxChar_Value));
	}
}

std::string CryptEnigma::crypt(const std::string &input_string)
{
	reset();
	std::string output_string;
	output_string.reserve(input_string.size());

	for (auto input_char : input_string)
	{
		output_string.push_back(cryptOneChar(input_char));
	}
	return output_string;
}

std::string CryptEnigma::encrypt(const std::string &s)
{
	reset();
	return crypt(s);
}

void CryptEnigma::reset()
{
	for (auto& r : mRotors)
	{
		r.reset();
	}
	mShiftedRotorIdx = 0;
}

uint8_t CryptEnigma::cryptOneChar(uint8_t input_char)
{
	// прямой ход
//	std::cout << "Start: " << input_char << " ";
	for (auto& rotor : mRotors)
	{
		input_char = rotor.crypt(input_char);
	}
//	std::cout << "Forward: " << input_char << " ";

	// отражение
	input_char = mReflector.crypt(input_char);
//	std::cout << "Reflect: " << input_char << " ";

	// обратный ход
	for (auto r_iter = mRotors.rbegin(); r_iter != mRotors.rend(); ++r_iter)
	{
		input_char = r_iter->encrypt(input_char);
	}
//	std::cout << "Back: " << input_char << std::endl;

	bool is_full_rotate = mRotors[mShiftedRotorIdx].shift();
	if (is_full_rotate == true)
	{
		mShiftedRotorIdx = (mShiftedRotorIdx + 1) % mRotors.size();
	}

	return input_char;
}

CryptEnigma::~CryptEnigma() = default;
