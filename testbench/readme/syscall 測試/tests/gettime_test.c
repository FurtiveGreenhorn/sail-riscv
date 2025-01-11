#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

int main() {
    
    struct timeval start, end;
    long seconds, useconds;    
    double duration;

    // 獲取起始時間
    gettimeofday(&start, NULL);
    for(int i=0; i<100000000; i++);
    // 假設有一些要執行的操作...
    // （這裡可以放置需要計時的代碼）
    // 獲取結束時間
    gettimeofday(&end, NULL);

    // 計算持續時間
    seconds = end.tv_sec - start.tv_sec; // 秒數差
    useconds = end.tv_usec - start.tv_usec; // 微秒數差
    duration = seconds + useconds / 1.0e6; // 總持續時間，單位為秒

    printf("time：%.6f sec\n", duration);

    return 0;
}


