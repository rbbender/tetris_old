#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <utility_func.h>



unsigned get_time() {
    static int basetime = 0;
    static int baseusec = 0;
    struct timeval tv;
    gettimeofday(&tv, NULL);
    if (basetime == 0) {
        basetime = tv.tv_sec;
        baseusec = tv.tv_usec;
    }
    unsigned res = (tv.tv_sec - basetime) * 1000000 + tv.tv_usec - baseusec;
    //printf("res=%f\n", res);
    return res;
}

unsigned long long get_current_time_ms() {
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
}
