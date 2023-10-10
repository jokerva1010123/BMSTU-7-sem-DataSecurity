#ifndef _KEY_GEN_HPP_
#define _KEY_GEN_HPP_

#include <gmpxx.h>

#define NUMBER_DEGREE 40
#define MUTUALLY_PRIME_NUM "65537"

#include <gmpxx.h>

std::pair<mpz_class, mpz_class> generate_large_prime_numbers();
std::tuple<mpz_class, mpz_class, mpz_class> keygen(const mpz_class& p, const mpz_class& q, const std::string& ei = "65537");

#endif
