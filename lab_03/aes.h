#ifndef _AES_H_
#define _AES_H_

#include <stdint.h>

void AES128_Encrypt(uint8_t* input, const uint8_t* key, uint8_t *output);

void AES128_Decrypt(uint8_t* input, const uint8_t* key, uint8_t *output);

#endif //_AES_H_
