/***************************************************************************
 *
 *   File        : eq_util.h
 *   Name        : Sangchul Lee (Scott)
 *
 ***************************************************************************/
#ifndef EQ_UTIL_H

#include <limits.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* structure which defines a timestamp */
typedef struct {
    int year;
    int month;
    int day;
    int hour;
    int min;
    int sec;
    int msec;
} timestamp_t;

/* the main structure holding the data for one earthquake */
typedef struct {
    timestamp_t* timestamp;
    float latitude;
    float longitude;
    float magnitude;
    float x;
    float y;
} eq_t;

void find_max_break_after_2000(void* c);
int time_compare(const void* a, const void* b);
/* helper function which maps (lat,long) to (x,y) */
void map_coordinates(eq_t* eq);
/* helper function which parses the timestamp in the input data */
timestamp_t* parse_time(char* buf);
/* helper function which reads data for one earthquake */
eq_t* read_quake(FILE* fp);
/* given two timestamps compute the difference in seconds */
double time_difference(timestamp_t* a, timestamp_t* b);
/* helper functions which wrap around standard functions and perform error handling */
void* safe_malloc(size_t bytes);
void* safe_realloc(void* ptr, size_t bytes);
size_t file_size_in_bytes(FILE* fp);
FILE* safe_open_file(const char* file_name, const char* mode);

/* helper function which print/free one eq_t object */
void eq_print(FILE* fp, eq_t* q);
void eq_free(void* q);
void score_free(void* q);

#endif
