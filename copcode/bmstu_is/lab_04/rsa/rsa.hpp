#ifndef _RSA_HPP_
#define _RSA_HPP_

#include <gmpxx.h>

constexpr const char* pubkey = "pubkey.rsa";
constexpr const char* prtkey = "prtkey.rsa";

// encrypt => crypt(message, e, n);
// decrypt => crypt(encrypted, d, n);
mpz_class crypt(const mpz_class& base, const mpz_class& exp, const mpz_class& mod);
#endif
