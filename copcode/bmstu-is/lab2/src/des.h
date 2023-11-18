#ifndef _DES_H_
#define _DES_H_

#include <stdint.h>

void DES_Decrypt(uint8_t *key, const char *input, const char *output);

void DES_Encrypt(uint8_t *key, const char *input, const char *output);

#endif  //_DES_H_