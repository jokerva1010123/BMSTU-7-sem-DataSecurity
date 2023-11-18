#include <argp.h>
#include <string.h>
#include <stdio.h>

#include "des.h"

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

  uint8_t key[8] = {0};

  printf("Enter key: ");
  fgets((char*) key, 8, stdin);

  if (arguments.mode == ENCRYPT) {
    DES_Encrypt(key, file_path, out_file_name);
  } else {
    DES_Decrypt(key, file_path, out_file_name);
  }

  return 0;
}
