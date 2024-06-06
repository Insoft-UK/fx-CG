//
//  common.h
//  macOS
//
//  Created by Richie on 31/05/2024.
//

#ifndef common_hpp
#define common_hpp

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

void strins(char *str, const char *insert, size_t pos, size_t length);
void strinc(char *str, const char c, size_t pos, size_t length);
void rmchrs(char *str, size_t pos, size_t chrs, size_t length);
void revstr(char* str, size_t length);
int itostr(int x, char* str, int d);
void trimr(char *str, const char c);
void dtostr(double num, char* str, int precision);

#ifdef __cplusplus
}
#endif

#endif /* common_hpp */
