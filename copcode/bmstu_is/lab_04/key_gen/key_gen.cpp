#include "key_gen.hpp"

#include <ctime>
#include <cassert>

void initialize_generator(gmp_randstate_t grt) {
    gmp_randinit_default(grt);
    gmp_randseed_ui(grt, time(nullptr));
}

std::pair<mpz_class, mpz_class> generate_large_prime_numbers() {
    gmp_randstate_t grt;
    initialize_generator(grt);

    mpz_class p;
    mpz_class q;

    mpz_urandomb(p.get_mpz_t(), grt, 20);
    mpz_urandomb(q.get_mpz_t(), grt, 20);

    mpz_nextprime(p.get_mpz_t(), p.get_mpz_t());
    mpz_nextprime(q.get_mpz_t(), q.get_mpz_t());

    return {p, q};
}

mpz_class eulers_totient_function(const mpz_class& p_minus_1, const mpz_class& q_minus_1) {
    return /* f = */ p_minus_1 * q_minus_1;
}

mpz_class carmichael_function(const mpz_class& p_minus_1, const mpz_class& q_minus_1) {
    mpz_class lambda;
    mpz_lcm(lambda.get_mpz_t(), p_minus_1.get_mpz_t(), q_minus_1.get_mpz_t());
    return lambda;
}

std::tuple<mpz_class, mpz_class, mpz_class> keygen(const mpz_class& p, const mpz_class& q, const std::string& ei) {
    const mpz_class e(ei);
    assert(e > 1);

    const mpz_class n = p * q;

    const mpz_class p_minus_1 = p - 1;
    const mpz_class q_minus_1 = q - 1;

    const mpz_class mod = carmichael_function(p_minus_1, q_minus_1);
    assert(e < mod);

    mpz_class gcd;
    mpz_gcd(gcd.get_mpz_t(), e.get_mpz_t(), mod.get_mpz_t());
    assert(gcd == 1);  // e and mod must be coprime

    mpz_class d;
    mpz_invert(d.get_mpz_t(), e.get_mpz_t(), mod.get_mpz_t());

    mpz_class calc = e * d % mod;
    assert(calc == 1);

    return {e, d, n};
}
