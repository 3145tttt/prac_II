#include <stdio.h>
#include <stdlib.h>

double ***transpose(double ***arr) {
    int row_number = 0;
    while (arr[row_number]) {
        ++row_number;
    }

    if (row_number == 0) {
        double ***answer = (double ***)calloc(1, sizeof(*answer));
        return answer;
    }

    int *row_len = (int *)calloc(row_number, sizeof(*row_len));
    int *row_len_max = (int *)calloc(row_number, sizeof(*row_len));
    for(int i = 0; i < row_number; ++i){
        int col_number = 0;
        while(arr[i][col_number]){
            ++col_number;
        }
        row_len[i] = col_number;
    }
    int cur_max = row_len[row_number - 1];
    for(int i = row_number - 1; i >= 0; --i) {
        if(cur_max < row_len[i]){
            cur_max = row_len[i];
        }
        row_len_max[i] = cur_max;
    }

    double ***fill = (double ***)calloc(row_number + 1, sizeof(*fill));
    for (int i = 0; i < row_number; ++i){
        fill[i] = (double **)calloc(cur_max + 1, sizeof(**fill));
        for(int j = 0; j < row_len_max[i]; ++j){
            double *tmp = (double *)calloc(1, sizeof(double));
            if (j < row_len[i]){
                *tmp = arr[i][j][0];
            }
            fill[i][j] = tmp;
        }
    }

    double ***answer = (double ***)calloc(cur_max + 1, sizeof(*answer));
    for (int i = 0; i < cur_max; ++i) {
        answer[i] = (double **)calloc(row_number + 1, sizeof(**answer));
    }
    for (int i = 0; i < row_number; ++i) {
        for(int j = 0; j < row_len_max[i]; ++j){
            answer[j][i] = fill[i][j];
        }
    }
    for (int i = 0; i < row_number; ++i) {
        free(fill[i]);
    }
    free(fill);
    free(row_len_max);
    free(row_len);
    return answer;
}