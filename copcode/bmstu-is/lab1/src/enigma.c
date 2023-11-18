
#include "enigma.h"

#include <stdio.h>
#include <stdlib.h>

#define encoder_decode(c) (ALPHABET[(c)])

uint8_t encoder_encode(const uint8_t c) {
  for (uint8_t i = 0; i < ALPHABET_SIZE; ++i) {
    if (ALPHABET[i] == c) return i;
  }
}

enigma_t* enigma_init() {
  enigma_t* enigma = (enigma_t*)malloc(sizeof(enigma_t));
  if (enigma == NULL) return NULL;

  enigma->counter = 0;

  enigma->rotors = (uint8_t**)malloc(sizeof(uint8_t*) * NUM_ROTORS);

  for (int i = 0; i < NUM_ROTORS; ++i) {
    enigma->rotors[i] = (uint8_t*)malloc(sizeof(uint8_t) * ALPHABET_SIZE);
    for (int j = 0; j < ALPHABET_SIZE; ++j) {
      enigma->rotors[i][j] = ROTORS[i][j];
    }
  }

  initial_shift(enigma, 0, R_1);
  initial_shift(enigma, 1, R_2);
  initial_shift(enigma, 2, R_3);

  return enigma;
}

void initial_shift(enigma_t* enigma, const uint8_t rn, const int count) {
  for (uint8_t i = 0; i < count; ++i) {
    enigma_rotor_shift(enigma, rn);
  }
}

void enigma_free(enigma_t* enigma) {
  for (int i = 0; i < NUM_ROTORS; ++i) {
    free(enigma->rotors[i]);
  }

  free(enigma->rotors);
  free(enigma);
}

void enigma_rotor_shift(enigma_t* enigma, const uint8_t rn) {
  uint8_t temp = enigma->rotors[rn][ALPHABET_SIZE - 1];
  for (int i = ALPHABET_SIZE - 1; i > 0; --i) {
    enigma->rotors[rn][i] = enigma->rotors[rn][i - 1];
  }

  enigma->rotors[rn][0] = temp;
}

uint8_t rotor_find(enigma_t* enigma, const uint8_t rn, const uint8_t c) {
  for (uint8_t i = 0; i < ALPHABET_SIZE; ++i) {
    if (enigma->rotors[rn][i] == c) return i;
  }
}

uint8_t enigma_encrypt(enigma_t* enigma, const uint8_t c) {
  uint8_t encoded_ch = encoder_encode(c);

  uint8_t new_code = PLUGBOARD[encoded_ch];

  for (int i = 0; i < NUM_ROTORS; ++i) {
    new_code = enigma->rotors[i][new_code];
  }

  new_code = REFLECTOR[new_code];

  for (int i = NUM_ROTORS - 1; i >= 0; --i) {
    new_code = rotor_find(enigma, i, new_code);
  }

  new_code = PLUGBOARD[new_code];

  uint64_t rotor_queue = 1;
  enigma->counter += 1;
  for (int i = 0; i < NUM_ROTORS; ++i) {
    if (enigma->counter % rotor_queue == 0) {
      enigma_rotor_shift(enigma, i);
    }
    rotor_queue *= ALPHABET_SIZE;
  }

  return encoder_decode(new_code);
}
