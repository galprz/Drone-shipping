#ifndef gil_lock_hpp
#define gil_lock_hpp
#include "thread_locks.hpp"
#include <boost/python.hpp>
#include <thread>

namespace Common {
/**
 * Represents a GIL handler for a specific thread. 
 */
class GilLock {
private:
    PyThreadState* state;

public:
    /**
     * Adds the created object to a global data structure that holds the lock
     * for each thread.
     */
    GilLock();

    /**
     * Unlocks the GIL, thus frees the python intereperter to continue executing
     * code in parallel.
     * Used to achive real multi-processor parallelism.
     * You should unlock the GIL every time you enter cpp code execution from python.
     */
    void unlock();

    /**
     * Locks the GIL.
     * Should be called upon every call to external python code.
     */
    void lock();

    /**
     * Locks the GIL if not locked, and removes this object from the global data
     * structure.
     */
    ~GilLock();
};
}

#endif /* gil_lock_hpp */
