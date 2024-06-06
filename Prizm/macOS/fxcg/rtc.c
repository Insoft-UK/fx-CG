/*
 Copyright © 2024 Insoft. All rights reserved.
 
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

#include "fxcg/rtc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>


typedef struct {
    struct timespec start_time;
} RTC_Timer;

static void RTC_StartTimer(RTC_Timer* timer) {
    clock_gettime(CLOCK_MONOTONIC, &timer->start_time);
}

int RTC_Reset( int r )
{
    return 0;
}


void RTC_SetDateTime(unsigned char* timestr) {
    struct tm tm;
    struct timeval tv;

    // Parse the input string
    if (strptime((const char*)timestr, "%Y-%m-%d %H:%M:%S", &tm) == NULL) {
        fprintf(stderr, "Failed to parse time string\n");
        return;
    }

    // Convert to time_t and set the timeval struct
    tv.tv_sec = mktime(&tm);
    tv.tv_usec = 0;

    // Set the system time
    if (settimeofday(&tv, NULL) != 0) {
        perror("settimeofday");
    }
}

void RTC_GetTime( unsigned int*hour, unsigned int*minute, unsigned int*second, unsigned int*millisecond )
{
    struct timespec ts;
        struct tm tm;
        clock_gettime(CLOCK_REALTIME, &ts);  // Get the current time
        localtime_r(&ts.tv_sec, &tm);  // Convert to local time
        
        *hour = tm.tm_hour;
        *minute = tm.tm_min;
        *second = tm.tm_sec;
        *millisecond = (unsigned int)(ts.tv_nsec / 1000000);  // Convert nanoseconds to milliseconds
}

int RTC_GetTicks(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (int)(ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
}

int RTC_Elapsed_ms( int start_value, int duration_in_ms )
{
    struct timespec end_time;
    clock_gettime(CLOCK_MONOTONIC, &end_time);
    
    unsigned long start_ms = start_value;
    unsigned long end_ms = end_time.tv_sec * 1000 + end_time.tv_nsec / 1000000;

    return end_ms - start_ms >= duration_in_ms;
}

