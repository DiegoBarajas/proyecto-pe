/*
    PROYECTO FINAL PROGRAMACION ESTRUCTURADA

    Autor: Diego Barajas
    Codigo estudiante: 219770192
    Asignatura: Programacion estructurada
    Maestra: SARA ESQUIVEL TORRES

    Repo: https://github.com/DiegoBarajas/proyecto-pe
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio.h>

/* Funcion para leer enteros */
int leerEntero(char *mensaje, int min, int max) {
    int valor;
    char buffer[100];

    while (1) {
        printf("%s", mensaje);

        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("[ ERROR ] Error al leer la entrada.\n");
            continue;
        }

        if (sscanf(buffer, "%d", &valor) != 1) {
            printf("[ ERROR ] Entrada invalida. Por favor, ingresa un numero entero.\n");
            continue;
        }

        if (min != max) {
            if (valor < min) {
                printf("[ ATENCION ] El valor debe ser mayor o igual a %d.\n", min);
                continue;
            }
            if (valor > max) {
                printf("[ ATENCION ] El valor debe ser menor o igual a %d.\n", max);
                continue;
            }
        }

        return valor;
    }
}



/* Funcion para leer floats */
float leerFloat(char *mensaje, float min, float max) {
    float valor;
    char buffer[100];

    while (1) {
        printf("%s", mensaje);

        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("[ ERROR ] Error al leer la entrada.\n");
            continue;
        }

        if (sscanf(buffer, "%f", &valor) != 1) {
            printf("[ ERROR ] Entrada invalida. Ingresa un numero decimal.\n");
            continue;
        }

        if (min != max) {
            if (valor < min) {
                printf("[ ATENCION ] Debe ser mayor o igual a %.2f.\n", min);
                continue;
            }
            if (valor > max) {
                printf("[ ATENCION ] Debe ser menor o igual a %.2f.\n", max);
                continue;
            }
        }

        return valor;
    }
}

/* Funcion que lee strings */
void leerString(char *dest, char *mensaje, int min, int max) {
    int len = 0;
    int ch;

    printf("%s", mensaje);
    fflush(stdout);
    dest[0] = '\0';

    while (1) {
        ch = _getch();

        if (ch == 13) {
            if (len < min) {
                printf("\nDebe ingresar al menos %d caracteres.\n", min);
                continue;
            }
            break;
        }

        if (ch == 8 && len > 0) {
            len--;
            dest[len] = '\0';
            printf("\b \b");
        }else if (ch >= 32 && ch <= 126 && len < max) {
            dest[len++] = (char)ch;
            dest[len] = '\0';
            printf("%c", ch);
        }

        printf("\r%s%s [ %d caracteres restantes ]", mensaje, dest, max-len);
        fflush(stdout);
    }

    dest[len] = '\0';
    printf("\n");
}

void getConsoleWidth(int *width) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        *width  = csbi.srWindow.Right  - csbi.srWindow.Left + 1;
    } else {
        *width  = 80;
    }
}

void printTitle(char *titulo) {
    int width;
    getConsoleWidth(&width);

    for(int i=0; i<width; i++)
        printf("=");

    printf("\n%s\n", titulo);

    for(int i=0; i<width; i++)
        printf("=");

    printf("\n");
}

/* Pausar */
void pause() {
    system("pause");
}

/* Limpiar consola */
void clear() {
    system("cls");
}

/* Salto de linea */
void nl() {
    printf("\n");
}
