/***************************************************************************
 *
 *   File        : main.c
 *   Name        : Sangchul Lee (Scott)
 *
 ***************************************************************************/

#include "eq_util.h"
#include "linkedlist.h"
#include "tasks.h"

#include <math.h>
#include <string.h>
#include <sys/time.h>
#define SEC2USEC 1000000.0

int main(int argc, char** argv)
{
    struct timeval start, stop;
    double sec;

    char* eq_csv_file = argv[1];
    int resolution = atoi(argv[2]);
    int K = atoi(argv[3]);

    gettimeofday(&start, NULL);
    task_1_find_longest_break_after_2k(eq_csv_file);
    gettimeofday(&stop, NULL);
    sec = (stop.tv_sec - start.tv_sec)
        + (stop.tv_usec - start.tv_usec) / SEC2USEC;
    printf("TASK 1: %.3f seconds\n", sec);

    gettimeofday(&start, NULL);
    task_2_compute_cell_scores(eq_csv_file, resolution, K);
    gettimeofday(&stop, NULL);
    sec = (stop.tv_sec - start.tv_sec)
        + (stop.tv_usec - start.tv_usec) / SEC2USEC;
    printf("TASK 2: %.3f seconds\n", sec);

    gettimeofday(&start, NULL);
    task_3_magnitude_statistics(eq_csv_file);
    gettimeofday(&stop, NULL);
    sec = (stop.tv_sec - start.tv_sec)
        + (stop.tv_usec - start.tv_usec) / SEC2USEC;
    printf("TASK 3: %.3f seconds\n", sec);

    return (EXIT_SUCCESS);
}
