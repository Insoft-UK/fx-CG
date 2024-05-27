/*
 Copyright © 2023 Insoft. All rights reserved.
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 */


#include "fxcg/file.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <regex>
#include <string>

#define SMEM "/Applications/CASIO/SMEM/"

static FILE *gFile[2] = {NULL, NULL};
static char _buf[PATH_MAX];


static int Bfile_StrLength(const unsigned short* string)
{
    int i;
    for (i=0; ; i++) {
        if (string[i] == 0) return i;
    }
}

static int Bfile_GetFreeHandle(void)
{
    for (int i = 0; i < 1; i++)
    {
        if (gFile[i] == NULL) return i;
    }
    return -1;
}

// MARK: - macOS System Simulator Functions

int Bfile_CloseFile_OS( int handle )
{
    if (gFile[handle] == NULL) return -1;
    fclose(gFile[handle]);
    return handle;
}

int Bfile_CreateEntry_OS( const unsigned short*filename, TCreateMode mode, size_t *size)
{
    char file[256];
    Bfile_NameToStr_ncpy(file, filename, Bfile_StrLength(filename) + 1);
    
    std::string s;
    s = file;
    s = std::regex_replace(s, std::regex(R"(\\\\fls0\\)"), SMEM);
    
    
    
    if (mode == kCreateMode_File) {
        int hFile = Bfile_GetFreeHandle();
        if (hFile >= 0) {
            gFile[hFile] = fopen(s.c_str(), "rb");
            if (gFile[hFile] == NULL) return -1;
            
            int length = (int)*size;
            while (length--) {
                fputc(0, gFile[hFile]);
            }
            fclose(gFile[hFile]);
            gFile[hFile] = NULL;
            return 0;
        }
    }
    
    if (mode == kCreateMode_Folder) {
       
    }
    
    return -1;
}
int Bfile_DeleteEntry( const unsigned short *filename )
{
    return -1;
}
int Bfile_RenameEntry( const unsigned short *oldpath, const unsigned short *newpath )
{
    return -1;
}
int Bfile_FindClose( int FindHandle )
{
    return -1;
}
int Bfile_FindFirst( const char *pathname, int *FindHandle, char *foundfile, void *fileinfo )
{
    return -1;
}
int Bfile_FindFirst_NON_SMEM( const char *pathname, int *FindHandle, char *foundfile, void *fileinfo )
{
    return -1;
}
int Bfile_FindNext( int FindHandle, char *foundfile, char *fileinfo )
{
    return -1;
}
int Bfile_FindNext_NON_SMEM( int FindHandle, char *foundfile, char *fileinfo )
{
    return -1;
}
int Bfile_GetFileSize_OS(int handle)
{
    if (gFile[handle] == NULL) return -1;
    int current_pos = (int)ftell(gFile[handle]);
    fseek(gFile[handle], 0, SEEK_END);
    int size = (int)ftell(gFile[handle]);
    fseek(gFile[handle], current_pos, SEEK_SET);
    return size;
}
int Bfile_OpenFile_OS(const unsigned short *filename, int mode, int null)
{
    int size = Bfile_StrLength(filename);
    Bfile_NameToStr_ncpy(_buf, filename, size);
    std::string s;
    s = _buf;
    s = std::regex_replace(s, std::regex(R"(^\\\\fls0\\)"), SMEM);
    s = std::regex_replace(s, std::regex(R"(\\)"), "/");
    
    int h = Bfile_GetFreeHandle();
    if (h == -1) return -1;
    gFile[h] = fopen((const char *)s.c_str(), "rb");
    
    return h;
}
int Bfile_ReadFile_OS( int handle, void *buf, int size, int readpos )
{
    if (gFile[handle] == NULL) return -1;
    if (readpos > -1)
        fseek(gFile[handle], readpos, SEEK_SET);
    
    return (int)fread(buf, size, 1, gFile[handle]);
}
int Bfile_SeekFile_OS( int handle, int pos )
{
    if (gFile[handle] == NULL) return -1;
    fseek(gFile[handle], pos, SEEK_SET);
    return (int)ftell(gFile[handle]);
}
int Bfile_TellFile_OS( int handle )
{
    if (gFile[handle] == NULL) return -1;
    return (int)ftell(gFile[handle]);
}
int Bfile_WriteFile_OS( int handle, const void *buf, int size )
{
    if (gFile[handle] == NULL) return -1;
    fwrite(buf, size, 1, gFile[handle]);
    return (int)ftell(gFile[handle]);
}
void Bfile_NameToStr_ncpy(char* dest, const unsigned short* source, size_t n)
{
    int i;
    for (i=0; i<n; i++) {
        dest[i] = source[i];
        if (dest[i] == 0)
            break;
    }
}
void Bfile_StrToName_ncpy(unsigned short *dest, const char *source, size_t n)
{
    int i;
    for (i=0; i<n; i++) {
        dest[i] = source[i];
        if (dest[i] == 0 || dest[i] == 0xFF)
            break;
    }
    dest[i] = 0;
}
int Bfile_Name_MatchMask( const short*mask, const short*filename  )
{
    return -1;
}
int Bfile_GetMediaFree_OS( unsigned short*media_id, int*freespace )
{
    return 16 << 20; // 16MB
}




