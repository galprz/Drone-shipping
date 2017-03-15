#ifndef read_write_lock_hpp
#define read_write_lock_hpp
#include <iostream>
#include <mutex>
#include <condition_variable>

namespace Common {
/**
 * This class represents a read-write lock with precedence to the writer.
 */
class RWLock {
private:
    std::mutex 				_shared;
    std::condition_variable _reader_cv;
    std::condition_variable _writer_cv;
    int 					_active_readers;
    int 					_waiting_writers;
    int 					_active_writers;

public:
    RWLock()
    : _shared(), _reader_cv(), _writer_cv(), _active_readers(0), _waiting_writers(0), _active_writers(0) {}

	/**
	 * Inserts a new reader to the system and tries to aquire the read lock.
	 * This is a blocking call.
	 */
    void read_lock();

	/**
	 * Removes the reader from the system.
	 */
    void read_unlock();

	/**
	 * Aquires the writer lock. Only one writer can be in the system at the same time.
	 * This is a blocking call.
	 */
    void write_lock();

	/**
	 * Removes the writer from the system.
	 */
    void write_unlock();
};
};

#endif /* read_write_lock_hpp */
