#include "rw_lock.hpp"
using namespace Common;

void RWLock::read_lock() {
    std::unique_lock<std::mutex> lk(_shared);
    while (_waiting_writers != 0)
        _reader_cv.wait(lk);
    ++_active_readers;
    lk.unlock();
}

void RWLock::read_unlock() {
    std::unique_lock<std::mutex> lk(_shared);
    --_active_readers;
    lk.unlock();
    _writer_cv.notify_one();
}

void RWLock::write_lock() {
    std::unique_lock<std::mutex> lk(_shared);
    ++_waiting_writers;
    while (_active_readers != 0 || _active_writers != 0)
        _writer_cv.wait(lk);
    ++_active_writers;
    lk.unlock();
}

void RWLock::write_unlock() {
    std::unique_lock<std::mutex> lk(_shared);
    --_waiting_writers;
    --_active_writers;
    if (_waiting_writers > 0)
        _writer_cv.notify_one();
    else
        _reader_cv.notify_all();
    lk.unlock();
}
