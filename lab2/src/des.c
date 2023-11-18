#include "des.h"

#include <stdio.h>

// get b-th bit from the number a and left shift to the c-th position
#define get_bit(a, b, c) \
  (((a[(b - 1) / 8] >> (7 - ((b - 1) % 8))) & 1) << (7 - c))

static const uint8_t sbox[8][4][16] = {
    {{14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
     {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
     {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
     {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}},

    {{15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
     {3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
     {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
     {13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}},

    {{10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
     {13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
     {13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
     {1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}},

    {{7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
     {13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
     {10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
     {3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}},

    {{2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
     {14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
     {4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
     {11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}},

    {{12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
     {10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
     {9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
     {4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}},

    {{4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
     {13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
     {1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
     {6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}},

    {{13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
     {1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
     {7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
     {2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}}};

static const uint8_t ip[64] = {
    58, 50, 42, 34, 26, 18, 10, 2, 60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6, 64, 56, 48, 40, 32, 24, 16, 8,
    57, 49, 41, 33, 25, 17, 9,  1, 59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5, 63, 55, 47, 39, 31, 23, 15, 7};

static const uint8_t inv_ip[64] = {
    40, 8, 48, 16, 56, 24, 64, 32, 39, 7, 47, 15, 55, 23, 63, 31,
    38, 6, 46, 14, 54, 22, 62, 30, 37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28, 35, 3, 43, 11, 51, 19, 59, 27,
    34, 2, 42, 10, 50, 18, 58, 26, 33, 1, 41, 9,  49, 17, 57, 25};

static const uint8_t pc_1[56] = {
    57, 49, 41, 33, 25, 17, 9,  1,  58, 50, 42, 34, 26, 18, 10, 2,  59, 51, 43,
    35, 27, 19, 11, 3,  60, 52, 44, 36, 63, 55, 47, 39, 31, 23, 15, 7,  62, 54,
    46, 38, 30, 22, 14, 6,  61, 53, 45, 37, 29, 21, 13, 5,  28, 20, 12, 4};

static const uint8_t pc_2[48] = {
    14, 17, 11, 24, 1,  5,  3,  28, 15, 6,  21, 10, 23, 19, 12, 4,
    26, 8,  16, 7,  27, 20, 13, 2,  41, 52, 31, 37, 47, 55, 30, 40,
    51, 45, 33, 48, 44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32};

void different_permutations(const uint8_t *arr, uint8_t *x, uint8_t *y,
                            int y_size) {
  for (int i = 0; i < y_size; i++) {
    y[i] = get_bit(x, arr[8 * i], 0) | get_bit(x, arr[8 * i + 1], 1) |
           get_bit(x, arr[8 * i + 2], 2) | get_bit(x, arr[8 * i + 3], 3) |
           get_bit(x, arr[8 * i + 4], 4) | get_bit(x, arr[8 * i + 5], 5) |
           get_bit(x, arr[8 * i + 6], 6) | get_bit(x, arr[8 * i + 7], 7);
  }
}

void left_rotation(char c[7], char x[7], int b) {
  if (b == 1) {
    for (unsigned i = 0; i < 7; i++) {
      x[i] = (c[i] << 1) | ((c[i + 1] >> 3) & 1);
      if (i == 6) {
        x[i] = (c[i] << 1) | ((c[0] >> 3) & 1);
      }
    }
  } else if (b == 2) {
    for (unsigned i = 0; i < 7; i++) {
      x[i] = (c[i] << 2) | ((c[i + 1] >> 2) & 0x3);
      if (i == 6) {
        x[i] = (c[i] << 2) | ((c[0] >> 2) & 0x3);
      }
    }
  }
}

void right_rotation(char c[7], char x[7], int b) {
  if (b == 1) {
    x[0] = (c[0] >> 1) | ((c[6] & 1) << 3);
    for (unsigned i = 1; i < 7; i++) {
      x[i] = (c[i] >> 1) | ((c[i - 1] & 1) << 3);
    }
  } else if (b == 2) {
    x[0] = (c[0] >> 2) | ((c[6] & 0x3) << 2);
    for (unsigned i = 1; i < 7; i++) {
      x[i] = (c[i] >> 2) | ((c[i - 1] & 0x3) << 2);
    }
  }
}

void adjoin(char c[7], char d[7], uint8_t y[7]) {
  for (unsigned i = 0; i < 3; i++) {
    y[i] = (c[2 * i] << 4) | (c[(2 * i) + 1] & 0xF);
  }
  y[3] = (c[6] << 4) | (d[0] & 0xF);
  y[4] = (d[1] << 4) | (d[2] & 0xF);
  y[5] = (d[3] << 4) | (d[4] & 0xF);
  y[6] = (d[5] << 4) | (d[6] & 0xF);
}

void key_scheduling(uint8_t init_key[8], uint8_t key[16][6]) {
  uint8_t x[7];
  different_permutations(pc_1, init_key, x, 7);
  // 4 bits
  char c[17][7], d[17][7];

  for (unsigned i = 0; i < 4; i++) {
    if (i < 3) {
      c[0][2 * i] = x[i] >> 4;
      c[0][(2 * i) + 1] = x[i] & 0xF;
    } else if (i == 3) {
      c[0][2 * i] = x[i] >> 4;
      d[0][0] = x[i] & 0xF;
    }
  }
  d[0][1] = x[4] >> 4;
  d[0][2] = x[4] & 0xF;
  d[0][3] = x[5] >> 4;
  d[0][4] = x[5] & 0xF;
  d[0][5] = x[6] >> 4;
  d[0][6] = x[6] & 0xF;

  int count = 1;
  for (count = 1; count < 17; count++) {
    if (count == 1 || count == 2 || count == 9 || count == 16) {
      left_rotation(c[count - 1], c[count], 1);
      left_rotation(d[count - 1], d[count], 1);
    } else {
      left_rotation(c[count - 1], c[count], 2);
      left_rotation(d[count - 1], d[count], 2);
    }
  }

  uint8_t y[16][7];

  for (unsigned step = 0; step < 16; step++) {
    adjoin(c[step + 1], d[step + 1], y[step]);
    different_permutations(pc_2, y[step], key[step], 6);
  }
}

void reverse_key_scheduling(uint8_t init_key[8], uint8_t key[16][6]) {
  uint8_t x[7];
  different_permutations(pc_1, init_key, x, 7);
  // 4 bits
  char c[17][7], d[17][7];

  for (unsigned i = 0; i < 4; i++) {
    if (i < 3) {
      c[0][2 * i] = x[i] >> 4;
      c[0][(2 * i) + 1] = x[i] & 0xF;
    } else if (i == 3) {
      c[0][2 * i] = x[i] >> 4;
      d[0][0] = x[i] & 0xF;
    }
  }
  d[0][1] = x[4] >> 4;
  d[0][2] = x[4] & 0xF;
  d[0][3] = x[5] >> 4;
  d[0][4] = x[5] & 0xF;
  d[0][5] = x[6] >> 4;
  d[0][6] = x[6] & 0xF;

  int count = 0;
  for (count = 1; count < 16; count++) {
    if (count == 1 || count == 8 || count == 15) {
      right_rotation(c[count - 1], c[count], 1);
      right_rotation(d[count - 1], d[count], 1);
    } else {
      right_rotation(c[count - 1], c[count], 2);
      right_rotation(d[count - 1], d[count], 2);
    }
  }

  uint8_t y[16][7];

  for (unsigned step = 0; step < 16; step++) {
    adjoin(c[step], d[step], y[step]);
    different_permutations(pc_2, y[step], key[step], 6);
  }
}

void xor_expand_key(uint8_t r[4], char ex[8], uint8_t key[6]) {
  uint8_t EXP[48] = {32, 1,  2,  3,  4,  5,  4,  5,  6,  7,  8,  9,
                     8,  9,  10, 11, 12, 13, 12, 13, 14, 15, 16, 17,
                     16, 17, 18, 19, 20, 21, 20, 21, 22, 23, 24, 25,
                     24, 25, 26, 27, 28, 29, 28, 29, 30, 31, 32, 1};

  uint8_t d[6];
  different_permutations(EXP, r, d, 6);

  uint8_t c[6];
  for (unsigned j = 0; j < 6; j++) {
    c[j] = d[j] ^ key[j];
  }

  // c[i] 8 bits -> ex[i] 6 bits for s_box
  ex[0] = c[0] >> 2;
  ex[1] = (c[0] & 0x3) | (((c[1] >> 4) & 0x0F) << 2);
  ex[2] = (c[1] & 0x0F) | (((c[2] >> 6) & 0x3) << 4);
  ex[3] = c[2] & 0x3F;
  ex[4] = c[3] >> 2;
  ex[5] = (c[3] & 0x3) | (((c[4] >> 4) & 0x0F) << 2);
  ex[6] = (c[4] & 0x0F) | (((c[5] >> 6) & 0x3) << 4);
  ex[7] = c[5] & 0x3F;
}

void apply_s_box_perm(char x[8], uint8_t f[4]) {
  char y[8];

  for (unsigned i = 0; i < 8; i++) {
    int col = (x[i] & 0x1E) >> 1;
    int row = (x[i] & 0x1) | ((x[i] >> 5) << 1);
    y[i] = sbox[i][row][col];
  }

  uint8_t e[4];

  for (unsigned j = 0; j < 4; j++) {
    e[j] = (y[2 * j] << 4) | y[(2 * j) + 1];
  }

  uint8_t permute[32] = {16, 7, 20, 21, 29, 12, 28, 17, 1,  15, 23,
                         26, 5, 18, 31, 10, 2,  8,  24, 14, 32, 27,
                         3,  9, 19, 13, 30, 6,  22, 11, 4,  25};

  different_permutations(permute, e, f, 4);
}

void encrypt_block(uint8_t m[8], uint8_t c[8], uint8_t k[8]) {
  uint8_t p[8];
  different_permutations(ip, m, p, 8);

  uint8_t l[4], r[4];
  for (unsigned i = 0; i < 4; i++) {
    l[i] = p[i];
  }
  for (unsigned j = 0; j < 4; j++) {
    r[j] = p[j + 4];
  }
  int count = 0;
  uint8_t key[16][6];
  key_scheduling(k, key);
  while (count < 16) {
    char expaned_text[8];
    xor_expand_key(r, expaned_text, key[count]);
    uint8_t f[4];
    apply_s_box_perm(expaned_text, f);
    uint8_t t[4];
    for (unsigned i = 0; i < 4; i++) {
      t[i] = l[i];
    }
    for (unsigned n = 0; n < 4; n++) {
      l[n] = r[n];
      r[n] = t[n] ^ f[n];
    }
    count++;
  }

  uint8_t temp[8];
  for (unsigned q = 0; q < 4; q++)  // last swap before inverse permutation
  {
    temp[q] = r[q];
  }
  for (unsigned r = 0; r < 4; r++) {
    temp[r + 4] = l[r];
  }

  different_permutations(inv_ip, temp, c, 8);
}

void DES_Encrypt(uint8_t *key, const char *input, const char *output) {
  FILE *file_in = fopen(input, "rb");
  if (file_in == NULL) return;

  FILE *file_out = fopen(output, "wb");
  if (file_out == NULL) {
    fclose(file_in);
    return;
  }

  fseek(file_in, 0L, SEEK_END);
  size_t size = ftell(file_in);
  fseek(file_in, 0L, SEEK_SET);

  uint8_t IV[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  uint8_t y[8];
  int first_read = 1;
  while (size > 0) {
    uint8_t plain_text[8] = {0};
    size_t bytes_readed = fread(plain_text, 1, 8, file_in);
    size -= bytes_readed;

    uint8_t ci[8] = {0};
    if (first_read) {
      encrypt_block(IV, ci, key);
      first_read = 0;
    } else {
      encrypt_block(y, ci, key);
    }

    for (unsigned i = 0; i < 8; i++) {
      y[i] = ci[i] ^ plain_text[i];
    }

    fwrite(y, bytes_readed, 1, file_out);
  }

  fclose(file_in);
  fclose(file_out);
}

void DES_Decrypt(uint8_t *key, const char *input, const char *output) {
  uint8_t x[8];

  FILE *file_in = fopen(input, "rb");
  if (file_in == NULL) return;

  FILE *file_out = fopen(output, "wb");
  if (file_out == NULL) {
    fclose(file_in);
    return;
  }

  fseek(file_in, 0L, SEEK_END);
  size_t size = ftell(file_in);
  fseek(file_in, 0L, SEEK_SET);

  uint8_t prev_cipher_text[8];
  int first_read = 1;
  while (size > 0) {
    uint8_t IV[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    uint8_t cipher_text[8];

    size_t bytes_readed = fread(cipher_text, 1, 8, file_in);
    size -= bytes_readed;

    uint8_t ci[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    if (first_read) {
      encrypt_block(IV, ci, key);
      first_read = 0;
    } else {
      encrypt_block(prev_cipher_text, ci, key);
    }

    for (unsigned i = 0; i < 8; i++) {
      x[i] = ci[i] ^ cipher_text[i];
      prev_cipher_text[i] = cipher_text[i];
    }

    fwrite(x, bytes_readed, 1, file_out);
  }

  fclose(file_in);
  fclose(file_out);
}
