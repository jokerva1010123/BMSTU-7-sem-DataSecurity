#include <stdio.h>

int main(void)
{
    char name[100];

    printf("Введите имя:\n");
    scanf("%s", &name);
    printf("Привет, %s!\n", name);
}
