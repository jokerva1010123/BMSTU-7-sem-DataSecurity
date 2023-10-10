#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#include "constants.h"

bool build_program_file(char license_key[LEN_LICENSE_KEY])
{
    char license[80] = "LICENSE_KEY=\"";
    bool success;
    pid_t child_id, child_status;
    int status;

    strncat(license, license_key, LEN_LICENSE_KEY);
    strncat(license, "\"", 2);

    child_id = fork();
    if (child_id == -1){
        return -1;
    }
    else if (child_id == 0){
        if (execlp("gcc", "gcc", "./program.c", "-D", license, "-o", "./program.out", NULL) == -1){
            perror("Ошибка создания файла программы. ");
            return -1;
        }
        return 0;
    }

    child_status = wait(&status);
    if (WIFEXITED(status)){
        success = true;
    }
    else {
        success = false;
    }

    return success;    
}

bool get_license_key(char license_key[LEN_LICENSE_KEY])
{
    FILE *license_file = fopen(LICENSE_PATH, "r");
    if (!license_file){
        return false;
    }

    fscanf(license_file, "%s", license_key);
    fclose(license_file);

    return true;
}

bool read_license_file(char license_key[LEN_LICENSE_KEY])
{   
    if (get_license_key(license_key) == false){
        return false;
    }

    return true;
}

int main(void)
{
    char license_key[LEN_LICENSE_KEY];

    if (read_license_file(license_key) == false){
        printf("Ошибка установки программы.\n");
        return ERROR_CREATE_LICENSE;
    }

    if (build_program_file(license_key) == false){
        printf("Ошибка установки программы.\n");
        return ERROR_BUILD_FILE;
    }
    else {
        printf("Программа установлена успешно.\n");
    }
        
    return EXIT_SUCCESS;
}