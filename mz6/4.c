#include <stdio.h>
#include <time.h>

enum
{
    START_YEAR = 1900,
    WEEKS_IN_YEAR = 52,
    DAYS_IN_WEEK = 7,
    TUESDAY_NUM = 4,
    EVEN_FLAG = 3
};

int main(){
    int year;
    scanf("%d", &year);
    struct tm cur_year = {0};
    
    cur_year.tm_year = year - START_YEAR;
    cur_year.tm_mday = 1;
    cur_year.tm_isdst = -1;
    mktime(&cur_year);
    cur_year.tm_mday += (TUESDAY_NUM - cur_year.tm_wday);
    for(int i = 0; i < WEEKS_IN_YEAR; ++i){
        cur_year.tm_mday += DAYS_IN_WEEK;
        cur_year.tm_isdst = -1;
        mktime(&cur_year);
        const int NUM_WEEK = (cur_year.tm_mday - 1)/ DAYS_IN_WEEK; 
        if(cur_year.tm_mday % EVEN_FLAG != 0 && (NUM_WEEK == 1 || NUM_WEEK == EVEN_FLAG))
            printf("%d %d\n", cur_year.tm_mon + 1, cur_year.tm_mday);
    }
    return 0;
}