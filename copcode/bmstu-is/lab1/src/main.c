#include <argp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "enigma.h"

const char *argp_program_version = "ENIGMA v1";
static char doc[] = "ENIGMA v1";
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

  FILE *file_in = fopen(file_path, "rb");
  if (file_in == NULL) {
    puts("Input file open problem");
    return 1;
  }

  FILE *file_out = fopen(out_file_name, "wb");
  if (file_out == NULL) {
    puts("Output file open problem");

    fclose(file_in);
    return 1;
  }

  enigma_t *enigma = enigma_init();
  if (enigma == NULL) {
    puts("Mem alloc failed");
    fclose(file_in);
    fclose(file_out);

    return 1;
  }

  char c;
  while (fread(&c, 1, sizeof(char), file_in) > 0) {
    uint8_t res = enigma_encrypt(enigma, (uint8_t)c);
    fwrite(&res, 1, sizeof(char), file_out);
  }

  enigma_free(enigma);
  fclose(file_in);
  fclose(file_out);

  return 0;
}
