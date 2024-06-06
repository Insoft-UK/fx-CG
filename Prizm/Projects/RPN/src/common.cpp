//
//  common.c
//  macOS
//
//  Created by Richie on 31/05/2024.
//

#include "common.hpp"
#include "fxCG/math.hpp"

#include <stdlib.h>
#include <string.h>


void strins(char *str, const char *insert, size_t pos, size_t length)
{
    size_t str_length = strlen(str);
    size_t insert_length = strlen(insert);
    
    if (pos > str_length) return;
    if (insert_length + str_length > length) return;
    
    memmove(&str[pos + insert_length], &str[pos], str_length + pos);
    strncpy(&str[pos], insert, insert_length);
}

void strinc(char *str, const char c, size_t pos, size_t length)
{
    size_t str_length = strlen(str);
    if (str_length + 1 >= length) return;
    memmove(&str[pos + 1], &str[pos], str_length - pos);
    str[pos] = c;
}

void rmchrs(char *str, size_t pos, size_t chrs, size_t length)
{
//    if (pos + chrs > length) return;
    memmove(&str[pos], &str[pos + 1], chrs);
}

void revstr(char* str, size_t length)
{
    if (!length) return;
    if (!*str) return;
    size_t start = 0;
    size_t end = length - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

int itostr(int num, char* str, int d)
{
    int i = 0;
    if (num < 0) num = -num;
    
    while (num) {
        str[i++] = (num % 10) + '0';
        num = num / 10;
    }

    // If number of digits required is more, add 0s
    while (i < d)
        str[i++] = '0';

    revstr(str, i);
    str[i] = '\0';
    return i;
}


void trimr(char *str, const char c)
{
    int i;
    
    // Start from the end of the string
    for (i = (int)strlen(str) - 1; i > 0; i--)
        if (str[i] != c)
            break;
        else
            str[i] = '\0';
}


void dtostr(double num, char* str, int precision)
{
    // Extract integer part
    int intPart = (int)num;

    // Extract floating part
    double floatPart = num - (double)intPart;

    // Convert integer part to string
    int i = itostr(intPart, str, 0);

    // Add dot
    if (precision != 0) {
        str[i] = '.';
        i++;
    }

    // Convert fractional part to string
    floatPart = floatPart * fxCG::math::pow(10, precision);
    itostr((int)floatPart, str + i, precision);
    
    if (strchr(str, '.')) {
        trimr(str, '0');
        trimr(str, '.');
    }
    
    if (num < 0) strinc(str, 0x87, 0, 50);
}
