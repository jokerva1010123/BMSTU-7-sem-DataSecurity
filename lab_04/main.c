#include <stdio.h>
#include <string.h>
#include "md5.h"
#include "rsa.h"

typedef enum { HASH = 0, ENCRYPT, DECRYPT } Modes;

struct arguments 
{
  Modes mode;
};

void print_hash(uint8_t p[RESULT_SIZE]) 
{
  for (int i = 0; i < RESULT_SIZE; ++i) 
    printf("%x", p[i]);
  printf("\n");
}

int main(const int argc, char *argv[]) 
{
  struct arguments arguments;
  if(argc == 3 && strcmp(argv[2], "h") == 0)
      arguments.mode = HASH;
  else 
    if (argc == 2 && strcmp(argv[1], "e") == 0)
      arguments.mode = ENCRYPT;
    else
      if (argc == 2 && strcmp(argv[1], "d") == 0)
        arguments.mode = DECRYPT;
      else
      {
        printf("Wrong mode!\n");
        return 0;
      }
  if (arguments.mode == HASH) 
  {
    uint8_t result[RESULT_SIZE];
    FILE *f = fopen(argv[1], "r");
    md5sum(f, result);
    fclose(f);
    print_hash(result);
  } 
  else if (arguments.mode == ENCRYPT) 
  {
    struct pub_key_t pub[1];
    struct priv_key_t priv[1];
    uint8_t hash[RESULT_SIZE] = {0};
    char s[RESULT_SIZE + 1] = {0};
    fgets(s, sizeof(s), stdin);
    s[strcspn(s, "\n")] = '\0';
    s[RESULT_SIZE + 1] = 0;
    for (int i = 0; i < RESULT_SIZE; ++i) 
    {
      hash[i] = s[i];
    }
    rsa_gen_keys(pub, priv);
    for (int i = 0; i < RESULT_SIZE; ++i) 
    {
      long long int encrypted = rsa_encrypt((long long int)hash[i], priv);
      printf("%lld ", encrypted);
    }
    printf("\n");
    printf("OK\n");
  } 
  else 
  {
    struct pub_key_t pub[1];
    struct priv_key_t priv[1];
    rsa_gen_keys(pub, priv);
    for (int i = 0; i < RESULT_SIZE; ++i) 
    {
      long long int encrypted;
      scanf("%lld", &encrypted);
      uint8_t decrypted = rsa_decrypt(encrypted, pub);
      printf("%c\n", decrypted);
    }
    printf("OK\n");
  }
  return 0;
}
