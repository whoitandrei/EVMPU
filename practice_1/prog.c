#include <stdio.h>
#include <sys\times.h>
#include <unistd.h>


double calculate(double x, long long N) {
    double tmp = x;
    double result = 0;
    for (int n = 0; n < N; n++) {
        result += tmp; 
        tmp *= -x * x / ((2 * n + 2) * (2 * n + 3));
    }
    return result;
}

int main(void){
    struct tms start, end;
    long clocks_per_sec = sysconf(_SC_CLK_TCK);
    long clocks;
    times(&start);
    
    double x = 3.1415;
    long long N;
    scanf("%lld", &N);
    printf("%lf\n", calculate(x, N));

    times(&end);
    clocks = end.tms_utime - start.tms_utime;
    printf("time: %lf", (double) clocks / clocks_per_sec);
    return 0;
}
