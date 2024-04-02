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
#include <string.h>
#include <stdlib.h>

#define SMEM "~/fx-CG/SMEM/"

static FILE *gFile[2] = {NULL, NULL};

static int Bfile_StrLength(const unsigned short* string)
{
    int length = 0;
    while (string[length++] != 0);
    return length-1;
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

int Bfile_CloseFile_OS( int HANDLE )
{
    if (gFile[HANDLE] == NULL) return -1;
    fclose(gFile[HANDLE]);
    return HANDLE;
}

int Bfile_CreateEntry_OS( const unsigned short*filename, TCreateMode mode, size_t *size)
{
    char file[256];
    Bfile_NameToStr_ncpy(file, filename, Bfile_StrLength(filename) + 1);
    
    if (mode == kCreateMode_File) {
        int hFile = Bfile_GetFreeHandle();
        if (hFile >= 0) {
            gFile[hFile] = fopen(file, "rb");
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
    return -1;
}
int Bfile_OpenFile_OS(const unsigned short *filename, TOpenMode mode, int null)
{
    return -1;
}
int Bfile_ReadFile_OS( int HANDLE, void *buf, int size, int readpos )
{
    return -1;
}
int Bfile_SeekFile_OS( int handle, int pos )
{
    return -1;
}
int Bfile_TellFile_OS( int handle )
{
    return -1;
}
int Bfile_WriteFile_OS( int HANDLE, const void *buf, int size )
{
    return -1;
}
void Bfile_NameToStr_ncpy(char* dest, const unsigned short* source, size_t n)
{
    while (n--) {
        *dest++ = *source++;
    }
}
void Bfile_StrToName_ncpy(unsigned short *dest, const char *source, size_t n)
{
    char *path = SMEM;
    for (unsigned long i = 0; i < strlen(path); i++)
    {
        *dest = (unsigned short)path[i];
    }
    while (n--) {
        *dest = (unsigned short)(*source);
        
        dest++;
        source++;
    }
}
int Bfile_Name_MatchMask( const short*mask, const short*filename  )
{
    return -1;
}
int Bfile_GetMediaFree_OS( unsigned short*media_id, int*freespace )
{
    return -1;
}




