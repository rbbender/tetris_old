#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <utility_func.h>

double get_time() {
    static int basetime = 0;
    struct timeval tv;
    gettimeofday(&tv, NULL);
    if (basetime == 0)
        basetime = tv.tv_sec;
    double res = (tv.tv_sec - basetime) + tv.tv_usec / 1000000;
    //printf("res=%f\n", res);
    return res;
}

bool kbhit() {
    int nreadbytes;
    ioctl(STDIN_FILENO, FIONREAD, &nreadbytes);
    return (nreadbytes > 0);
}