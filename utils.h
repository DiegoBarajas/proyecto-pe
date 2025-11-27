/*
    PROYECTO FINAL PROGRAMACION ESTRUCTURADA

    Autor: Diego Barajas
    Codigo estudiante: 219770192
    Asignatura: Programacion estructurada
    Maestra: SARA ESQUIVEL TORRES

    Repo: https://github.com/DiegoBarajas/proyecto-pe
*/
#include <string.h>
#include <ctype.h>

void toUpper(char *str) {
    for (int i = 0; str[i]; i++)
        str[i] = toupper((unsigned char)str[i]);
}
