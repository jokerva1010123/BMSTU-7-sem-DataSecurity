#ifndef __RSA_H__
#define __RSA_H__

struct pub_key_t {
  long long modulus;
  long long exponent;
};

struct priv_key_t {
  long long modulus;
  long long exponent;
};

// Prime numbers
#define P 61031
#define Q 32633

void rsa_gen_keys(struct pub_key_t* pub, struct priv_key_t* priv);
long long rsa_encrypt(const long long data, const struct priv_key_t* priv);
long long rsa_decrypt(const long long data, const struct pub_key_t* pub);

#endif  //__RSA_H__
