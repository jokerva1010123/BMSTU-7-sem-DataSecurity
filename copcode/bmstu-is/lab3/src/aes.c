#include "aes.h"

#include <string.h>

// Nb
#define BLOCK_SIZE 4
// Nk
#define KEY_SIZE 4
// 128bit = 10, 192bit = 12 ...
#define ROUNDS 10
// 128bit
#define AES_BLOCK_LEN 16

typedef struct {
  uint8_t round_key[176];
  uint8_t iv[AES_BLOCK_LEN];
} ctx_t;

typedef uint8_t state_t[4][4];

// lookup-tables
static const uint8_t sbox[256] = {
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b,
    0xfe, 0xd7, 0xab, 0x76, 0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0,
    0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0, 0xb7, 0xfd, 0x93, 0x26,
    0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2,
    0xeb, 0x27, 0xb2, 0x75, 0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0,
    0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84, 0x53, 0xd1, 0x00, 0xed,
    0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f,
    0x50, 0x3c, 0x9f, 0xa8, 0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5,
    0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2, 0xcd, 0x0c, 0x13, 0xec,
    0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14,
    0xde, 0x5e, 0x0b, 0xdb, 0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c,
    0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79, 0xe7, 0xc8, 0x37, 0x6d,
    0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f,
    0x4b, 0xbd, 0x8b, 0x8a, 0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e,
    0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e, 0xe1, 0xf8, 0x98, 0x11,
    0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f,
    0xb0, 0x54, 0xbb, 0x16};

static const uint8_t rsbox[256] = {
    0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e,
    0x81, 0xf3, 0xd7, 0xfb, 0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87,
    0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb, 0x54, 0x7b, 0x94, 0x32,
    0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
    0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49,
    0x6d, 0x8b, 0xd1, 0x25, 0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16,
    0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92, 0x6c, 0x70, 0x48, 0x50,
    0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
    0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05,
    0xb8, 0xb3, 0x45, 0x06, 0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02,
    0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b, 0x3a, 0x91, 0x11, 0x41,
    0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
    0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8,
    0x1c, 0x75, 0xdf, 0x6e, 0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89,
    0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b, 0xfc, 0x56, 0x3e, 0x4b,
    0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
    0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59,
    0x27, 0x80, 0xec, 0x5f, 0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d,
    0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef, 0xa0, 0xe0, 0x3b, 0x4d,
    0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63,
    0x55, 0x21, 0x0c, 0x7d};

// round constant word array
static const uint8_t rcon[11] = {0x8d, 0x01, 0x02, 0x04, 0x08, 0x10,
                                 0x20, 0x40, 0x80, 0x1b, 0x36};

#define get_sbox_value(i) (sbox[(i)])
#define get_sbox_invert(i) (rsbox[(i)])

void expand_key(uint8_t* round_key, const uint8_t* Key) {
  unsigned i, j, k;
  uint8_t temp[4];

  for (i = 0; i < KEY_SIZE; ++i) {
    round_key[(i * 4) + 0] = Key[(i * 4) + 0];
    round_key[(i * 4) + 1] = Key[(i * 4) + 1];
    round_key[(i * 4) + 2] = Key[(i * 4) + 2];
    round_key[(i * 4) + 3] = Key[(i * 4) + 3];
  }

  for (i = KEY_SIZE; i < BLOCK_SIZE * (ROUNDS + 1); ++i) {
    k = (i - 1) * 4;
    temp[0] = round_key[k + 0];
    temp[1] = round_key[k + 1];
    temp[2] = round_key[k + 2];
    temp[3] = round_key[k + 3];

    if (i % KEY_SIZE == 0) {
      // [0,1,2,3] -> [1,2,3,0]
      const uint8_t u8tmp = temp[0];
      temp[0] = temp[1];
      temp[1] = temp[2];
      temp[2] = temp[3];
      temp[3] = u8tmp;

      temp[0] = get_sbox_value(temp[0]);
      temp[1] = get_sbox_value(temp[1]);
      temp[2] = get_sbox_value(temp[2]);
      temp[3] = get_sbox_value(temp[3]);

      temp[0] = temp[0] ^ rcon[i / KEY_SIZE];
    }

    j = i * 4;
    k = (i - KEY_SIZE) * 4;
    round_key[j + 0] = round_key[k + 0] ^ temp[0];
    round_key[j + 1] = round_key[k + 1] ^ temp[1];
    round_key[j + 2] = round_key[k + 2] ^ temp[2];
    round_key[j + 3] = round_key[k + 3] ^ temp[3];
  }
}

void init_iv(ctx_t* ctx, const uint8_t* key, const uint8_t* iv) {
  expand_key(ctx->round_key, key);
  memcpy(ctx->iv, iv, AES_BLOCK_LEN);
}

static void add_round_key(uint8_t round, state_t* state,
                          const uint8_t* round_key) {
  uint8_t i, j;
  for (i = 0; i < 4; ++i) {
    for (j = 0; j < 4; ++j) {
      (*state)[i][j] ^=
          round_key[(round * BLOCK_SIZE * 4) + (i * BLOCK_SIZE) + j];
    }
  }
}

static void sub_bytes(state_t* state) {
  uint8_t i, j;
  for (i = 0; i < 4; ++i) {
    for (j = 0; j < 4; ++j) {
      (*state)[j][i] = get_sbox_value((*state)[j][i]);
    }
  }
}

static void shift_rows(state_t* state) {
  uint8_t temp;

  // Rotate first row 1 columns to left
  temp = (*state)[0][1];
  (*state)[0][1] = (*state)[1][1];
  (*state)[1][1] = (*state)[2][1];
  (*state)[2][1] = (*state)[3][1];
  (*state)[3][1] = temp;

  // Rotate second row 2 columns to left
  temp = (*state)[0][2];
  (*state)[0][2] = (*state)[2][2];
  (*state)[2][2] = temp;

  temp = (*state)[1][2];
  (*state)[1][2] = (*state)[3][2];
  (*state)[3][2] = temp;

  // Rotate third row 3 columns to left
  temp = (*state)[0][3];
  (*state)[0][3] = (*state)[3][3];
  (*state)[3][3] = (*state)[2][3];
  (*state)[2][3] = (*state)[1][3];
  (*state)[1][3] = temp;
}

static uint8_t xtime(uint8_t x) { return ((x << 1) ^ (((x >> 7) & 1) * 0x1b)); }

static void mix_columns(state_t* state) {
  uint8_t i;
  uint8_t Tmp, Tm, t;
  for (i = 0; i < 4; ++i) {
    t = (*state)[i][0];
    Tmp = (*state)[i][0] ^ (*state)[i][1] ^ (*state)[i][2] ^ (*state)[i][3];
    Tm = (*state)[i][0] ^ (*state)[i][1];
    Tm = xtime(Tm);
    (*state)[i][0] ^= Tm ^ Tmp;
    Tm = (*state)[i][1] ^ (*state)[i][2];
    Tm = xtime(Tm);
    (*state)[i][1] ^= Tm ^ Tmp;
    Tm = (*state)[i][2] ^ (*state)[i][3];
    Tm = xtime(Tm);
    (*state)[i][2] ^= Tm ^ Tmp;
    Tm = (*state)[i][3] ^ t;
    Tm = xtime(Tm);
    (*state)[i][3] ^= Tm ^ Tmp;
  }
}

#define multiply(x, y)                         \
  (((y & 1) * x) ^ ((y >> 1 & 1) * xtime(x)) ^ \
   ((y >> 2 & 1) * xtime(xtime(x))) ^          \
   ((y >> 3 & 1) * xtime(xtime(xtime(x)))) ^   \
   ((y >> 4 & 1) * xtime(xtime(xtime(xtime(x))))))

static void inv_mix_columns(state_t* state) {
  int i;
  uint8_t a, b, c, d;
  for (i = 0; i < 4; ++i) {
    a = (*state)[i][0];
    b = (*state)[i][1];
    c = (*state)[i][2];
    d = (*state)[i][3];

    (*state)[i][0] = multiply(a, 0x0e) ^ multiply(b, 0x0b) ^ multiply(c, 0x0d) ^
                     multiply(d, 0x09);
    (*state)[i][1] = multiply(a, 0x09) ^ multiply(b, 0x0e) ^ multiply(c, 0x0b) ^
                     multiply(d, 0x0d);
    (*state)[i][2] = multiply(a, 0x0d) ^ multiply(b, 0x09) ^ multiply(c, 0x0e) ^
                     multiply(d, 0x0b);
    (*state)[i][3] = multiply(a, 0x0b) ^ multiply(b, 0x0d) ^ multiply(c, 0x09) ^
                     multiply(d, 0x0e);
  }
}

static void inv_sub_bytes(state_t* state) {
  uint8_t i, j;
  for (i = 0; i < 4; ++i) {
    for (j = 0; j < 4; ++j) {
      (*state)[j][i] = get_sbox_invert((*state)[j][i]);
    }
  }
}

static void inv_shift_rows(state_t* state) {
  uint8_t temp;

  // Rotate first row 1 columns to right
  temp = (*state)[3][1];
  (*state)[3][1] = (*state)[2][1];
  (*state)[2][1] = (*state)[1][1];
  (*state)[1][1] = (*state)[0][1];
  (*state)[0][1] = temp;

  // Rotate second row 2 columns to right
  temp = (*state)[0][2];
  (*state)[0][2] = (*state)[2][2];
  (*state)[2][2] = temp;

  temp = (*state)[1][2];
  (*state)[1][2] = (*state)[3][2];
  (*state)[3][2] = temp;

  // Rotate third row 3 columns to right
  temp = (*state)[0][3];
  (*state)[0][3] = (*state)[1][3];
  (*state)[1][3] = (*state)[2][3];
  (*state)[2][3] = (*state)[3][3];
  (*state)[3][3] = temp;
}

void encrypt_plaintext(state_t* state, const uint8_t* round_key) {
  uint8_t round = 0;

  add_round_key(0, state, round_key);

  for (round = 1;; ++round) {
    sub_bytes(state);
    shift_rows(state);
    if (round == ROUNDS) {
      break;
    }
    mix_columns(state);
    add_round_key(round, state, round_key);
  }
  add_round_key(ROUNDS, state, round_key);
}

void inv_encrypt_plaintext(state_t* state, const uint8_t* round_key) {
  uint8_t round = 0;

  add_round_key(ROUNDS, state, round_key);

  for (round = (ROUNDS - 1);; --round) {
    inv_shift_rows(state);
    inv_sub_bytes(state);
    add_round_key(round, state, round_key);
    if (round == 0) {
      break;
    }
    inv_mix_columns(state);
  }
}

static void xor_block(uint8_t* buf, const uint8_t* iv) {
  for (uint8_t i = 0; i < AES_BLOCK_LEN; ++i) {
    buf[i] ^= iv[i];
  }
}

void encrypt_block(ctx_t* ctx, uint8_t* buf, uint32_t length) {
  uintptr_t i;
  uint8_t* iv = ctx->iv;
  for (i = 0; i < length; i += AES_BLOCK_LEN) {
    xor_block(buf, iv);
    encrypt_plaintext((state_t*)buf, ctx->round_key);
    iv = buf;
    buf += AES_BLOCK_LEN;
  }
  /* store iv in ctx for next call */
  memcpy(ctx->iv, iv, AES_BLOCK_LEN);
}

void decrypt_block(ctx_t* ctx, uint8_t* buf, uint32_t length) {
  uintptr_t i;
  uint8_t next_iv[AES_BLOCK_LEN];
  for (i = 0; i < length; i += AES_BLOCK_LEN) {
    memcpy(next_iv, buf, AES_BLOCK_LEN);
    inv_encrypt_plaintext((state_t*)buf, ctx->round_key);
    xor_block(buf, ctx->iv);
    memcpy(ctx->iv, next_iv, AES_BLOCK_LEN);
    buf += AES_BLOCK_LEN;
  }
}

void AES128_Encrypt(uint8_t* input, const uint8_t* key, uint8_t* output) {
  ctx_t ctx;
  uint8_t iv[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  init_iv(&ctx, key, iv);
  uint8_t buf[16];
  for (int i = 0; i < 16; i++) buf[i] = input[i];

  encrypt_block(&ctx, buf, 16);

  for (int i = 0; i < 16; i++) output[i] = buf[i];
}

void AES128_Decrypt(uint8_t* input, const uint8_t* key, uint8_t* output) {
  ctx_t ctx;
  uint8_t iv[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  init_iv(&ctx, key, iv);
  uint8_t buf[16];
  for (int i = 0; i < 16; i++) buf[i] = input[i];

  decrypt_block(&ctx, buf, 16);

  for (int i = 0; i < 16; i++) output[i] = buf[i];
}
