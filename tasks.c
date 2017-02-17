/***************************************************************************
 *
 *   File        : tasks.c
 *   Name        : Sangchul Lee (Scott)
 ***************************************************************************/

#include "tasks.h"
#include "eq_util.h"
#include "linkedlist.h"

#define MAX_BUF_LEN 2048
#define INITIAL_SIZE 100
#define FIRST_YEAR 1985
#define LAST_YEAR 2015
#define YEAR_2000 2000
#define MAGNITUDE_4 4.0
#define MAGNITUDE_5 5.0
#define MAGNITUDE_6 6.0
#define MAGNITUDE_7 7.0
#define MAGNITUDE_8 8.0
#define MAGNITUDE_9 9.0

void task_1_find_longest_break_after_2k(const char* eq_csv_file)
{
    FILE* fp = safe_open_file(eq_csv_file, "r");
    int buf_size = INITIAL_SIZE;
    eq_t** eq_list = (eq_t**)safe_malloc(sizeof(eq_t*) * buf_size);
    int lines_read = 0;
    eq_list[lines_read] = read_quake(fp);
    while (eq_list[lines_read]) {
        if (lines_read + 1 == buf_size) {
            buf_size = buf_size * 2;
            eq_list = safe_realloc(eq_list, sizeof(eq_t*) * buf_size);
        } else {
            lines_read++;
            eq_list[lines_read] = read_quake(fp);
        }
    }

    eq_list = safe_realloc(eq_list, lines_read * sizeof(eq_t*));
    fclose(fp);

    qsort(eq_list, lines_read, sizeof(eq_t*), time_compare);
    FILE* task_1 = safe_open_file("task_1.csv", "w");
    fprintf(task_1, "timestamp,latitude,longitude,magnitude\n");

    double max = fabs(time_difference(eq_list[lines_read - 2]->timestamp, eq_list[lines_read - 1]->timestamp));
    eq_t* max_eq_1;
    eq_t* max_eq_2;
    for (int i = 0; i < lines_read - 1; i++) {
        if (eq_list[i]->timestamp->year >= YEAR_2000) {
            timestamp_t* temp1 = eq_list[i]->timestamp;
            timestamp_t* temp2 = eq_list[i + 1]->timestamp;
            double time_diff = fabs(time_difference(temp1, temp2));
            if (time_diff > max) {
                max = time_diff;
                max_eq_1 = eq_list[i];
                max_eq_2 = eq_list[i + 1];
            }
        }
    }
    eq_print(task_1, max_eq_1);
    fprintf(task_1, "\n");
    eq_print(task_1, max_eq_2);
    fprintf(task_1, "\n");

    fclose(fp);
    for (int i = 0; i < lines_read; i++) {
        eq_free(eq_list[i]);
    }
    free(eq_list);
}

void task_2_compute_cell_scores(const char* eq_csv_file, int resolution, int K)
{
    FILE* fp = safe_open_file(eq_csv_file, "r");

    list_t*** eq_grid = (list_t***)safe_malloc(resolution * sizeof(list_t**));
    for (int i = 0; i < resolution; i++) {
        eq_grid[i] = (list_t**)safe_malloc(resolution * sizeof(list_t*));
        for (int j = 0; j < resolution; j++)
            eq_grid[i][j] = NULL;
    }
    eq_t* new_eq = read_quake(fp);
    while (new_eq) {
        int ix = new_eq->x * resolution;
        int iy = new_eq->y * resolution;
        if (eq_grid[ix][iy] == NULL) {
            eq_grid[ix][iy] = list_new(eq_free);
            list_insert_in_order(eq_grid[ix][iy], new_eq, magnitude_cmp);
        } else {
            list_insert_in_order(eq_grid[ix][iy], new_eq, magnitude_cmp);
        }
        new_eq = read_quake(fp);
    }

    score_t* score_array = (score_t*)safe_malloc(resolution * resolution * sizeof(score_t));
    int l = 0;
    for (int i = 0; i < resolution; i++) {
        for (int j = 0; j < resolution; j++) {
            double cell_score = compute_score(eq_grid[i][j]);
            score_array[l].score = cell_score;
            if (eq_grid[i][j] != NULL) {
                score_array[l].x = eq_grid[i][j]->head->data;
            } else {
                score_array[l].x = NULL;
            }
            l++;
        }
    }
    qsort(score_array, resolution * resolution, sizeof(score_t), score_cmp);
    FILE* task_2 = safe_open_file("task_2.csv", "w");
    fprintf(task_2, "timestamp,latitude,longitude,magnitude,score\n");

    for (int i = K - 1; i >= 0; i--) {
        if (score_array[i].x == NULL) {
            printf("error\n");
            exit(0);
        }
        eq_print(task_2, score_array[i].x);
        fprintf(task_2, ",%.2f\n", score_array[i].score);
    }

    fclose(fp);

    for (int i = 0; i < resolution; i++) {
        for (int j = 0; j < resolution; j++) {
            if (eq_grid[i][j] != NULL) {
                list_free(eq_grid[i][j]);
            }
        }
        free(eq_grid[i]);
    }
    free(eq_grid);
    free(score_array);
}

void task_3_magnitude_statistics(const char* eq_csv_file)
{
    FILE* fp = safe_open_file(eq_csv_file, "r");
    int buf_size = INITIAL_SIZE;
    eq_t** eq_list = (eq_t**)safe_malloc(sizeof(eq_t*) * buf_size);
    int lines_read = 0;
    eq_list[lines_read] = read_quake(fp);
    while (eq_list[lines_read]) {
        if (lines_read + 1 == buf_size) {
            buf_size = buf_size * 2;
            eq_list = safe_realloc(eq_list, sizeof(eq_t*) * buf_size);
        } else {
            lines_read++;
            eq_list[lines_read] = read_quake(fp);
        }
    }

    eq_list = safe_realloc(eq_list, lines_read * sizeof(eq_t*));
    fclose(fp);

    FILE* task_3 = safe_open_file("task_3.csv", "w");
    fprintf(task_3, "year,magnitude,count,percentage\n");

    int eq_mag_4;
    int eq_mag_5;
    int eq_mag_6;
    int eq_mag_7;
    int eq_mag_8;
    int eq_mag_9;
    int eq_mag_over_9;
    int total_count;

    double eq_mag_4_per;
    double eq_mag_5_per;
    double eq_mag_6_per;
    double eq_mag_7_per;
    double eq_mag_8_per;
    double eq_mag_9_per;
    double eq_mag_over_9_per;

    for (int i = FIRST_YEAR; i <= LAST_YEAR; i++) {
        eq_mag_4 = 0;
        eq_mag_5 = 0;
        eq_mag_6 = 0;
        eq_mag_7 = 0;
        eq_mag_8 = 0;
        eq_mag_9 = 0;
        eq_mag_over_9 = 0;

        for (int j = 0; j < lines_read; j++) {
            if (eq_list[j]->timestamp->year == i) {
                if (eq_list[j]->magnitude < MAGNITUDE_4)
                    eq_mag_4++;
                else if (eq_list[j]->magnitude < MAGNITUDE_5)
                    eq_mag_5++;
                else if (eq_list[j]->magnitude < MAGNITUDE_6)
                    eq_mag_6++;
                else if (eq_list[j]->magnitude < MAGNITUDE_7)
                    eq_mag_7++;
                else if (eq_list[j]->magnitude < MAGNITUDE_8)
                    eq_mag_8++;
                else if (eq_list[j]->magnitude < MAGNITUDE_9)
                    eq_mag_9++;
                else if (eq_list[j]->magnitude >= MAGNITUDE_9)
                    eq_mag_over_9++;
            }
        }
        total_count = eq_mag_4 + eq_mag_5 + eq_mag_6 + eq_mag_7 + eq_mag_8
            + eq_mag_9 + eq_mag_over_9;
        eq_mag_4_per = (double)eq_mag_4 / (double)total_count * 100;
        eq_mag_5_per = (double)eq_mag_5 / (double)total_count * 100 + eq_mag_4_per;
        eq_mag_6_per = (double)eq_mag_6 / (double)total_count * 100 + eq_mag_5_per;
        eq_mag_7_per = (double)eq_mag_7 / (double)total_count * 100 + eq_mag_6_per;
        eq_mag_8_per = (double)eq_mag_8 / (double)total_count * 100 + eq_mag_7_per;
        eq_mag_9_per = (double)eq_mag_9 / (double)total_count * 100 + eq_mag_8_per;
        eq_mag_over_9_per = (double)eq_mag_over_9 / (double)total_count * 100 + eq_mag_9_per;

        fprintf(task_3, "%d,%d,%d,%.1f\n", i, 3, eq_mag_4, eq_mag_4_per);
        fprintf(task_3, "%d,%d,%d,%.1f\n", i, 4, eq_mag_5, eq_mag_5_per);
        fprintf(task_3, "%d,%d,%d,%.1f\n", i, 5, eq_mag_6, eq_mag_6_per);
        fprintf(task_3, "%d,%d,%d,%.1f\n", i, 6, eq_mag_7, eq_mag_7_per);
        fprintf(task_3, "%d,%d,%d,%.1f\n", i, 7, eq_mag_8, eq_mag_8_per);
        fprintf(task_3, "%d,%d,%d,%.1f\n", i, 8, eq_mag_9, eq_mag_9_per);
        fprintf(task_3, "%d,%d,%d,%.1f\n", i, 9, eq_mag_over_9, eq_mag_over_9_per);
    }
    fclose(fp);

    for (int i = 0; i < lines_read; i++) {
        if (eq_list[i] != NULL) {
            eq_free(eq_list[i]);
        }
    }
    free(eq_list);
}
