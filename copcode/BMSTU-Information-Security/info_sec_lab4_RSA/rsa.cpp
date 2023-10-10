#include "rsa.h"
#include "debug.h"
#include "utils.h"
#include <assert.h>

constexpr uint64_t BlockSize = 32;

string RSA::crypt(const string &str)
{
	if (mKeysGenerated == false)
	{
		generateKeys(16);
	}
	string binCipherText;
	for (uint64_t i = 0, n = str.size(); i < n; ++i)
	{
		uint8_t cur_byte = str.at(i);
		uint64_t cipher_byte = moduloDegree(cur_byte, mPublicKey.first, mPublicKey.second);
		string bin_cipher_byte = _uintToBin(cipher_byte);
		bin_cipher_byte = supplement(bin_cipher_byte, BlockSize);
		binCipherText += bin_cipher_byte;
		Debug::debug("% crypt to % sub_str: %", static_cast<uint16_t>(cur_byte), cipher_byte, bin_cipher_byte);
	}
	string res = _binStrToSymbols(binCipherText, 8);
	Debug::debug("Res: %", res);
	return res;
}

string RSA::encrypt(const string &str)
{
	string res;
	string bin_str;
	for (uint8_t c: str)
	{
		string tmp = _uintToBin(c);
		tmp = supplement(tmp, 8);
		bin_str += tmp;
	}
	assert(bin_str.size() % BlockSize == 0);
	int64_t count = bin_str.size() / BlockSize;
	for (int i = 0; i < count; ++i)
	{
		string cur_substr = bin_str.substr(i * BlockSize, BlockSize);
		int64_t cur_cipher = _binStrToUlong(cur_substr);
		auto aaa = moduloDegree(cur_cipher, mSecretKey.first, mSecretKey.second);
		Debug::debug("Value Aaaaa: %", aaa);
		assert(aaa < 256);
//		uint8_t init_substr = moduloDegree(cur_cipher, mSecretKey.first, mSecretKey.second);
		res += aaa;
		Debug::debug("% encrypt to % substr %", cur_cipher, static_cast<uint16_t>(aaa), cur_substr);
	}

	Debug::debug("Res: %", res);
	return res;
}

void RSA::generateKeys(const uint16_t min)
{
	int64_t E;
	int64_t D;
	uint64_t fi;
	do {
		uint64_t P = PrimeNumberGenerator::generate(min, std::numeric_limits<uint16_t>::max());
		uint64_t Q = PrimeNumberGenerator::generate(min, std::numeric_limits<uint16_t>::max());
		mN = P * Q; // длина алфавита
		Debug::debug("P: %, Q: %, mN: %", P, Q, mN);
		fi = (P - 1) * (Q - 1);
		E = findE(fi);
		D = findD(fi, E, mN);

		mPublicKey.first = E;
		mPublicKey.second = mN;
		mSecretKey.first = D;
		mSecretKey.second = mN;

		Debug::debug("P: %", P);
		Debug::debug("Q: %", Q);
		Debug::debug("fi: %", fi);
		Debug::debug("E: %", E);
		Debug::debug("N: %", mN);

	} while (((E * D % fi) != 1));
	Debug::info("public key = <%, %>", E, mN);
	Debug::info("secret key = <%, %>", D, mN);
	mKeysGenerated = true;
}

int64_t RSA::findD(int64_t fi, int64_t E, int64_t N)
{
	vector<int64_t> ext_gcf = extendedEuclideanAlgorithm(fi, E);
	Debug::debug("NOD: %, alpha: %, beta %", ext_gcf[0], ext_gcf[1], ext_gcf[2]);
	Debug::debug("GCD(Fi, E) = %", ext_gcf[0]);
	int64_t D = ext_gcf[2];
	if (D < 0)
	{
		Debug::debug("D is negative. %, %", D, N);
//		D = D + N;
		D = D + fi;
	}
	Debug::debug("D is positive. %", D);

	return D;
}

int64_t RSA::findE(int64_t fi)
{
	int64_t gcf = 0;
	int64_t E;
	for ( ; gcf != 1; )
	{
		E = 2 + random() % (fi - 2);
		gcf = EuclideanAlgorithm(E, fi);
		Debug::debug("E: %, fi: %, gcf: %", E, fi, gcf);
	}
	return E;
}

/**
 * @brief Нахождение НОД
 * @param a
 * @param b
 * @return
 */
int64_t RSA::EuclideanAlgorithm(int64_t a, int64_t b)
{
	assert(a != 0 && b != 0);
	if (a == b)
	{
		return a;
	}
	int64_t r;
	while (b != 0)
	{
		r = a % b;
		a = b;
		b = r;
	}
	return a;
}

int64_t gcd (int64_t a, int64_t b, int64_t & x, int64_t & y)
{
	if (a == 0) {
		x = 0; y = 1;
		return b;
	}
	int64_t x1 = 0, y1 = 0;
	int64_t d = gcd (b % a, a, x1, y1);
	x = y1 - (b / a) * x1;
	y = x1;
	return d;
}

/**
 * @brief Нахождение НОД и коэффициентов x, y таких, что a * x + b * y = НОД(a, b).
 * @param a
 * @param b
 * @return [НОД, x, y]
 */
vector<int64_t> RSA::extendedEuclideanAlgorithm(int64_t a, int64_t b)
{
	assert(a > 0 && b > 0);
	int64_t x = 0, y = 0;
	int64_t NOD;
	NOD = gcd(a, b, x, y);
	return vector<int64_t>({NOD, x, y});
}
//	**************
//	int64_t old_r = a, r = b;
//	int64_t old_s = 1, s = 0;
//	int64_t old_t = 0, t = 1;

//	while (r != 0)
//	{
//		int64_t quotient = old_r / r;
//		int64_t prov = r;
//		r = old_r - quotient * prov;
//		old_r = prov;

//		prov = s;
//		s = old_s - quotient * prov;
//		old_s = prov;

//		prov = t;
//		t = old_t - quotient * prov;
//		old_t = prov;
//	}
//	int64_t _min = min(old_s, old_t);
//	int64_t _max = max(old_s, old_t);
//	vector<int64_t> res;
//	if (a > b)
//	{
//		res = {old_r, _min, _max};
//	}
//	else
//	{
//		res = {old_r, _max, _min};
//	}
//	return res;
//}

uint64_t RSA::moduloDegree(uint64_t base, uint64_t degree, uint64_t mod)
{
	uint64_t r = 1;
	while (degree > 0)
	{
		if (degree % 2 == 1) // если нечетное
		{
			r = (r * base) % mod;
		}
		base = (base * base) % mod;
		degree /= 2; // degree = degree / 2
	}
	return r;
}
