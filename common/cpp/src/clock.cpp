#include "clock.hpp"
using namespace Common;

void Clock::sleep(long milliseconds) {
    long milliseconds_past = 1000 * (clock() - last_sleep) / CLOCKS_PER_SEC;
    if (milliseconds - milliseconds_past > 0) {
        boost::this_thread::sleep(boost::posix_time::milliseconds(milliseconds - milliseconds_past));
    }
    last_sleep = clock();
}
