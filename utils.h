#include <string.h>
#include <ctype.h>

void toUpper(char *str) {
    for (int i = 0; str[i]; i++)
        str[i] = toupper((unsigned char)str[i]);
}
