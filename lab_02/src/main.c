// Динь Вьет Ань - ИУ7И-74Б
#include <string.h>
#include <stdio.h>

#include "des.h"

typedef enum { ENCRYPT = 0, DECRYPT } Modes;

struct arguments 
{
  Modes mode;
};

int main(const int argc, char **argv) 
{
  char file_name[100];
  char *file_path = argv[1];
  strcpy(file_name, argv[1]);

  struct arguments arguments;
  if (strcmp(argv[2], "e") == 0)
    arguments.mode = ENCRYPT;
  else if(strcmp(argv[2], "d") == 0)
    arguments.mode = DECRYPT;
  else
  {
    printf("Wrong mode\n");
    return 0 ;
  }
   char out_file_name[1000];
  if (arguments.mode == ENCRYPT) 
  {
    strcpy(out_file_name, "enc_");
    strcat(out_file_name, file_name);
  } 
  else 
  {
    strcpy(out_file_name, "dec_");
    strcat(out_file_name, file_name);
  }
  uint8_t key[8] = {0};

  printf("Enter key: ");
  fgets((char*) key, 8, stdin);

  if (arguments.mode == ENCRYPT) 
  {
    DES_Encrypt(key, file_path, out_file_name);
  } 
  else 
  {
    DES_Decrypt(key, file_path, out_file_name);
  }
  printf("OK!");
  return 0;
}
