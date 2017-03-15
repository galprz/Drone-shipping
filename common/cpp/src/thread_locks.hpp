#ifndef thread_locks_hpp
#define thread_locks_hpp
#include <iostream>
#include <thread>
#include <unordered_map>
#include "rw_lock.hpp"
#include "gil_lock.hpp"

/**
 * Locks the GIL for the specific thread.
 * Should be called upon every call to an external python code.
 */
#define BEGIN_PYTHON_EXECUTION                                      \
    ::Common::ThreadLocks::get(std::this_thread::get_id())->lock(); \
    {

/**
 * Unlocks the GIL for the specific thread.
 * Should be called after every call to an external pyhton code.
 */
#define END_PYTHON_EXECUTION \
    }                        \
    ::Common::ThreadLocks::get(std::this_thread::get_id())->unlock();

namespace Common {
class GilLock;

/**
 * This static class provides an interface to store and retreive the GIL handler
 * for every thread.
 */
class ThreadLocks {
private:
    static RWLock 									_read_write_lock;
    static std::unordered_map< size_t, GilLock* > 	_locks;
    ThreadLocks(){};

public:
    /**
	 * Adds a new GIL handler for a thread
	 * @param key  The thread id
	 * @param lock The GIL handler
	 */
    static void add(const std::thread::id& key, GilLock* lock);

    /**
	 * Gets the GIL handler for a thread
	 * @param  key The thread id
	 * @return     The GIL handler
	 */
    static GilLock* get(const std::thread::id& key);

    /**
	 * Removes a GIL handler for a thread
	 * @param key The thread id
	 */
    static void remove(const std::thread::id& key);
};
};

#endif /* thread_locks_hpp */
