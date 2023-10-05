#!/bin/bash

MACHINE_ID_FILE='/var/lib/dbus/machine-id'
TEMPORARY_FILE='program.c'
TEMPORARY_OBJ='program.o'
PROGRAM='program.exe'

MACHINE_ID_LEN=32

# get key
KEY=`cat $MACHINE_ID_FILE`

# create program text
PROGRAM_TEXT=`cat <<_EOF_
#include <stdio.h>
#include <string.h>

int fib32(int n);

int main(void)
{
    char key[$MACHINE_ID_LEN];

    FILE *file;

    if ((file = fopen("$MACHINE_ID_FILE", "r")) == NULL)
    {
        puts("fopen() error.");
        return 1;
    }

    fscanf(file, "%s", key);
    fclose(file);

    if (!strcmp(key, "$KEY"))
    {
        int n;
        printf("Input Fibonacci number ordinal: ");
        scanf("%d", &n);

        int num = fib32(n);

        printf("%d Fibonacci number is %d.", n, num);
        puts("");
    }
    else
    {
        puts("You do not have access rights.");
    }

    return 0;
}

_EOF_`

# create program
echo "$PROGRAM_TEXT" > $TEMPORARY_FILE
gcc -c $TEMPORARY_FILE
rm $TEMPORARY_FILE
gcc -o $PROGRAM $TEMPORARY_OBJ -L. -lnumbers
rm $TEMPORARY_OBJ
