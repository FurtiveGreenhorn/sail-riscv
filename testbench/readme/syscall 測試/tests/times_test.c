#include <sys/times.h>
#include <unistd.h>
#include <stdio.h>

int main() {
    struct tms start_times, end_times;
    clock_t start, end;
    //double ticks_per_second = sysconf(_SC_CLK_TCK);
    double ticks_per_second = 100;
    double user_time, system_time;

    // 獲取開始時間
    start = times(&start_times);

    // 這裡執行一些模擬耗時操作
    for (long i = 0; i < 1000000; i++) {
        // 模擬操作
    }

    // 獲取結束時間
    end = times(&end_times);

    // 計算用戶時間和系統時間
    user_time = (double)(end_times.tms_utime - start_times.tms_utime) / ticks_per_second;
    system_time = (double)(end_times.tms_stime - start_times.tms_stime) / ticks_per_second;

    // 輸出結果
    printf("user time: %f s\n", user_time);
    printf("system time: %f s\n", system_time);

    return 0;
}
