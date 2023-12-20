#include "rsa.h"

char buffer[1024];
const int MAX_DIGITS = 50;

long long gcd(long long a, long long b) 
{
  long long c;
  while (a != 0) 
  {
    c = a;
    a = b % a;
    b = c;
  }
  return b;
}

// d = 1/e (f)
// 

long long ext_euclid(long long a, long long b) 
{
  long long x = 0, y = 1, u = 1, v = 0, gcd = b, m, n, q, r;
  while (a != 0) 
  {
    q = gcd / a;
    r = gcd % a;
    m = x - u * q;
    n = y - v * q;
    gcd = a;
    a = r;
    x = u;
    y = v;
    u = m;
    v = n;
  }
  return y;
}

static inline long long mod_mult(long long a, long long b, long long mod) 
{
  if (a == 0) 
    return 0;
  long long sum = 0;
  while (b > 0) 
  {
    if (b & 1) sum = (sum + a) % mod;
    a = (2 * a) % mod;
    b >>= 1;
  }
  return sum;
}

long long mod_exp(long long b, long long e, long long m) 
{
  long long product;
  product = 1;
  if (b < 0 || e < 0 || m <= 0) 
    return -1;
  b = b % m;
  while (e > 0) 
  {
    if (e & 1)
      product = mod_mult(product, b, m);
    b = mod_mult(b, b, m);
    e >>= 1;
  }
  return product;
}

void rsa_gen_keys(struct pub_key_t* pub, struct priv_key_t* priv) 
{
  long long max = 0, phi_max = 0;
  long long e = (2 << 16) + 1;
  max = P * Q;
  phi_max = (P - 1) * (Q - 1);
  long long d = ext_euclid(phi_max, e);
  while (d < 0) 
    d = d + phi_max;
  pub->modulus = max;
  pub->exponent = e;
  priv->modulus = max;
  priv->exponent = d;
}

long long rsa_encrypt(const long long data, const struct priv_key_t* priv) 
{
  return mod_exp(data, priv->exponent, priv->modulus);
}

long long rsa_decrypt(const long long data, const struct pub_key_t* pub) 
{
  return mod_exp(data, pub->exponent, pub->modulus);
}
