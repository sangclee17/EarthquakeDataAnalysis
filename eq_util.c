/***************************************************************************
 *
 *   File        : eq_util.c
 *   Name        : Sangchul Lee (Scott)
 *
 ***************************************************************************/

#include "eq_util.h"

#include "linkedlist.h"

#define PI 3.141593
#define ZOOM 0
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MONTHSPERYEAR 12 /* months per calendar year */

/* print a timestamp */
void time_print(FILE* fp, timestamp_t* t)
{
    fprintf(fp, "%d-%02d-%02dT%02d:%02d:%02d.%03dZ\n",
        t->year, t->month, t->day, t->hour, t->min, t->sec, t->msec);
}

/* needed for time comparison */
long long my_timegm(struct tm* ptm)
{
    int m, y = ptm->tm_year;
    if ((m = ptm->tm_mon) < 2) {
        m += 12;
        --y;
    }
    return ((((long long)(y - 69) * 365u + y / 4 - y / 100 * 3 / 4 + (m + 2) * 153 / 5 - 446 + ptm->tm_mday) * 24u + ptm->tm_hour) * 60u + ptm->tm_min) * 60u + ptm->tm_sec;
}

double
time_difference(timestamp_t* a, timestamp_t* b)
{
    struct tm ta;
    struct tm tb;
    ta.tm_hour = a->hour;
    ta.tm_min = a->min;
    ta.tm_sec = a->sec;
    ta.tm_mon = a->month - 1;
    ta.tm_mday = a->day;
    ta.tm_year = a->year - 1900;
    ta.tm_isdst = 0;
    tb.tm_hour = b->hour;
    tb.tm_min = b->min;
    tb.tm_sec = b->sec;
    tb.tm_mon = b->month - 1;
    tb.tm_mday = b->day;
    tb.tm_year = b->year - 1900;
    tb.tm_isdst = 0;
    long long at = my_timegm(&ta);
    long long bt = my_timegm(&tb);
    double seconds = (double)(at - bt);
    return seconds;
}

int time_compare(const void* a, const void* b)
{
    eq_t** ttemp_a = (eq_t**)a;
    eq_t** ttemp_b = (eq_t**)b;

    eq_t* temp_a = *ttemp_a;
    eq_t* temp_b = *ttemp_b;

    double time_cmp = time_difference(temp_a->timestamp, temp_b->timestamp);
    if (time_cmp < 0)
        return -1;
    if (time_cmp > 0)
        return 1;
    return 0;
}

/* translates lat,long to x,y. ensure all x,y are in [0,1] */
void map_coordinates(eq_t* eq)
{
    float sinlat = sinf(eq->latitude * (PI / 180.0));
    sinlat = MIN(MAX(sinlat, -0.9999), 0.99625);
    float x = (0.5 + eq->longitude / 360);
    float y = (0.5 - logf((1.0 + sinlat) / (1.0 - sinlat)) / (4.0 * PI));
    y = y - 1;
    y *= -1;
    eq->x = MIN(x, 0.999);
    eq->y = MIN(y, 0.999);
}

/* a wrapper function around malloc which checks return values and exits on error */
void* safe_malloc(size_t bytes)
{
    void* ptr = malloc(bytes);
    if (ptr == NULL) {
        printf("ERROR: malloc(%lu)\n", bytes);
        exit(EXIT_FAILURE);
    }
    return ptr;
}

/* a wrapper function around realloc which checks return values and exits on error */
void* safe_realloc(void* ptr, size_t bytes)
{
    void* new_ptr = realloc(ptr, bytes);
    if (new_ptr == NULL) {
        printf("ERROR: realloc(%lu)\n", bytes);
        exit(EXIT_FAILURE);
    }
    return new_ptr;
}

/* function whcih parses the timestamp part of the input data */
timestamp_t* parse_time(char* buf)
{
    int year, month, day, hour, min, sec, msec;
    if (sscanf(buf, "%d-%d-%dT%d:%d:%d.%dZ", &year, &month, &day, &hour, &min, &sec, &msec) != 7) {
        printf("error parsing timestamp.\n");
        exit(EXIT_FAILURE);
    }

    timestamp_t* ts = (timestamp_t*)safe_malloc(sizeof(timestamp_t));
    ts->year = year;
    ts->month = month;
    ts->day = day;
    ts->hour = hour;
    ts->min = min;
    ts->sec = sec;
    ts->msec = msec;
    return ts;
}

/* a wrapper function around fopen which checks return values and exits on error */
FILE* safe_open_file(const char* file_name, const char* mode)
{
    FILE* fp = fopen(file_name, mode);
    if (fp == NULL) {
        perror("file open error.");
        exit(EXIT_FAILURE);
    }
    return fp;
}

/* a helper function which creates an eq_t object. returns NULL if everything has been read */
eq_t* read_quake(FILE* fp)
{
    char buf[1024];
    float latitude, longitude, magnitude;
    if (fscanf(fp, "%[^,],%f,%f,%f\n", buf, &latitude, &longitude, &magnitude) == 4) {
        eq_t* eq = (eq_t*)safe_malloc(sizeof(eq_t));
        eq->timestamp = parse_time(buf);
        eq->latitude = latitude;
        eq->longitude = longitude;
        eq->magnitude = magnitude;
        map_coordinates(eq);
        return eq;
    }
    return NULL;
}

/* helper function which frees all memory associated with an eq_t object */
void eq_free(void* q)
{
    eq_t* eq = (eq_t*)q;
    free(eq->timestamp);
    free(eq);
}

void score_free(void* q)
{
    score_t* score = (score_t*)q;
    free(score->x);
    free(score);
}

/* prints an eq_t object in the standard format required for the output files */
void eq_print(FILE* fp, eq_t* q)
{
    fprintf(fp, "%d-%02d-%02dT%02d:%02d:%02d.%03dZ,%.3f,%.3f,%.1f",
        q->timestamp->year,
        q->timestamp->month,
        q->timestamp->day,
        q->timestamp->hour,
        q->timestamp->min,
        q->timestamp->sec,
        q->timestamp->msec,
        q->latitude,
        q->longitude,
        q->magnitude);
}
