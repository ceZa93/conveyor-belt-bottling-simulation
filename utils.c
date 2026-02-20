#include "utils.h"

void int_to_str(uint8_t num, char *str)
{
    uint8_t i = 0;
    uint8_t j;
    char temp[4];   // max "255"

    if (num == 0) {
        str[0] = '0';
        str[1] = '\0';
        return;
    }

    while (num > 0) {
        temp[i++] = (num % 10) + '0';
        num /= 10;
    }

    // obrni redosled
    for (j = 0; j < i; j++) {
        str[j] = temp[i - j - 1];
    }

    str[i] = '\0';
}
