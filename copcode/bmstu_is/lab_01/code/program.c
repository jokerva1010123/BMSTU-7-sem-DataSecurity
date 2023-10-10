#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "constants.h"

#ifdef LICENSE_KEY
    bool get_license_key(char license_key[LEN_LICENSE_KEY], char *file_name)
    {
        FILE *license_file = NULL;

        license_file = fopen(file_name, "r");
        if (license_file == NULL){
            return false;
        }

        fscanf(license_file, "%s", license_key);
        fflush(license_file);
        fclose(license_file);

        return true;
    }

    bool check_license(void)
    {
        char current_key[LEN_LICENSE_KEY];
        
        if (get_license_key(current_key, LICENSE_PATH) == false){
            return false;
        }

        if (strcmp(current_key, LICENSE_KEY) != 0){
            return false;
        }    

        return true;
    }
#endif

int main(void)
{
    printf("Проверка лицензии...\n");

    if (check_license() == false){
        printf("Ошибка лицензии.");
    }
    else{
        printf("Запущена программа ...\n");
    }

    return 0;
}
