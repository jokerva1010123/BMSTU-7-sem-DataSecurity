#include <argp.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

// stdint.h
#include "aes.h"
#define BLOCK_SIZE 16

const char *argp_program_version = "AES PCBC v1";
const char *argp_program_bug_address = "artyombogachenco@gmail.com";
static char doc[] = "AES PCBC";
static char args_doc[] = "TARGET --encrypt --decrypt";
static struct argp_option options[] = {
    {"encrypt", 'e', 0, 0, "Encryption mode"},
    {"decrypt", 'd', 0, 0, "Decryption mode"},
    {0}};

typedef enum { ENCRYPT = 0, DECRYPT } Modes;

struct arguments {
  Modes mode;
};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
  struct arguments *arguments = state->input;
  switch (key) {
    case 'e':
      arguments->mode = ENCRYPT;
      break;
    case 'd':
      arguments->mode = DECRYPT;
      break;
    case ARGP_KEY_ARG:
      return 0;
    default:
      return ARGP_ERR_UNKNOWN;
  }
  return 0;
}

static struct argp argp = {options, parse_opt, args_doc, doc, 0, 0, 0};

int file_exist(char *filename) {
  FILE *file = fopen(filename, "rb");
  if (file) {
    fclose(file);
    return true;
  } else {
    return false;
  }
}

void xor_block(uint8_t *b1, uint8_t *b2, uint8_t *output) {
  for (int i = 0; i < BLOCK_SIZE; i++) output[i] = b1[i] ^ b2[i];
}

void padding(uint8_t *block, size_t size) {
  if (size < BLOCK_SIZE) {
    block[size] = 0x80;
    for (int i = size + 1; i < BLOCK_SIZE; i++) block[i] = 0x00;
  }
}

void encrypt(uint8_t *key, char *file_name_in, char *file_name_out) {
  FILE *file_in = fopen(file_name_in, "rb");
  FILE *file_out = fopen(file_name_out, "wb");

  fseek(file_in, 0L, SEEK_END);
  size_t file_size_bytes = ftell(file_in);
  fseek(file_in, 0L, SEEK_SET);

  uint8_t iv[BLOCK_SIZE] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  while (file_size_bytes > 0) {
    uint8_t plaintext_block[BLOCK_SIZE] = {0, 0, 0, 0, 0, 0, 0, 0,
                                           0, 0, 0, 0, 0, 0, 0, 0};

    size_t block_bytes = fread(plaintext_block, 1, BLOCK_SIZE, file_in);
    file_size_bytes -= block_bytes;
    if (block_bytes < BLOCK_SIZE) padding(plaintext_block, block_bytes);

    uint8_t iv_xor_pt[BLOCK_SIZE] = {0, 0, 0, 0, 0, 0, 0, 0,
                                     0, 0, 0, 0, 0, 0, 0, 0};
    uint8_t encrypted_block[BLOCK_SIZE] = {0, 0, 0, 0, 0, 0, 0, 0,
                                           0, 0, 0, 0, 0, 0, 0, 0};

    xor_block(iv, plaintext_block, iv_xor_pt);
    AES128_Encrypt(iv_xor_pt, key, encrypted_block);

    fwrite(encrypted_block, BLOCK_SIZE, 1, file_out);
    xor_block(encrypted_block, plaintext_block, iv);
  }
}

void decrypt(uint8_t *key, char *file_name_in, char *file_name_out) {
  FILE *file_in = fopen(file_name_in, "rb");
  FILE *file_out = fopen(file_name_out, "wb");

  fseek(file_in, 0L, SEEK_END);
  size_t file_size_bytes = ftell(file_in);
  fseek(file_in, 0L, SEEK_SET);

  uint8_t iv[BLOCK_SIZE] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  while (file_size_bytes > 0) {
    uint8_t encrypted_block[BLOCK_SIZE] = {0, 0, 0, 0, 0, 0, 0, 0,
                                           0, 0, 0, 0, 0, 0, 0, 0};

    size_t block_bytes = fread(encrypted_block, 1, BLOCK_SIZE, file_in);
    file_size_bytes -= block_bytes;

    uint8_t iv_xor_pt[BLOCK_SIZE] = {0, 0, 0, 0, 0, 0, 0, 0,
                                     0, 0, 0, 0, 0, 0, 0, 0};
    AES128_Decrypt(encrypted_block, key, iv_xor_pt);

    uint8_t plaintext_block[BLOCK_SIZE] = {0, 0, 0, 0, 0, 0, 0, 0,
                                           0, 0, 0, 0, 0, 0, 0, 0};
    xor_block(iv_xor_pt, iv, plaintext_block);

    if (file_size_bytes == 0) {
      for (int i = BLOCK_SIZE - 1; i >= 0; --i)
        if (plaintext_block[i] == 0x80 && i != 0)
          fwrite(plaintext_block, i, 1, file_out);
    } else {
      fwrite(plaintext_block, BLOCK_SIZE, 1, file_out);
      xor_block(encrypted_block, plaintext_block, iv);
    }
  }
}

void prepend(char *s, const char *t) {
  size_t len = strlen(t);

  memmove(s + len, s, strlen(s) + 1);
  memcpy(s, t, len);
}

int main(const int argc, char **argv) {
  char out_file_name[100];
  char *file_path = argv[1];
  strcpy(out_file_name, argv[1]);

  struct arguments arguments;
  arguments.mode = ENCRYPT;

  argp_parse(&argp, argc, argv, 0, 0, &arguments);

  if (arguments.mode == ENCRYPT) {
    strcat(out_file_name, ".enc");
  } else {
    out_file_name[(strlen(out_file_name) - 4)] = 0;
    prepend(out_file_name, "decrypted_");
  }

  if (!file_exist(file_path)) {
    printf("File %s doesn't exists!\n", file_path);
    return 0;
  }

  uint8_t key[16] = {0};

  printf("Enter key: ");
  fgets((char *)key, BLOCK_SIZE, stdin);

  if (arguments.mode == ENCRYPT) {
    encrypt(key, file_path, out_file_name);
  } else {
    decrypt(key, file_path, out_file_name);
  }

  return 0;
}
