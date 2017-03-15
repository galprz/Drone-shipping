#ifndef clock_hpp
#define clock_hpp
#include <ctime>
#include <boost/thread/thread.hpp>

namespace Common {
class Clock {
private:
    clock_t last_sleep;

public:
    Clock()
    : last_sleep(clock()) {}
    inline void reset() {
        last_sleep = clock();
    }
    void sleep(long milliseconds);
};
}

#endif /* clock_hpp */
