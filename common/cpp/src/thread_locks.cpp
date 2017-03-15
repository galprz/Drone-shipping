#include "thread_locks.hpp"
using namespace Common;

RWLock ThreadLocks::_read_write_lock;
std::unordered_map< size_t, GilLock* > ThreadLocks::_locks;

void ThreadLocks::add( const std::thread::id& key, GilLock* lock ) {
    std::hash< std::thread::id > hasher;
    size_t hashed_key = hasher( key );
    _read_write_lock.write_lock();
    _locks[ hashed_key ] = lock;
    _read_write_lock.write_unlock();
}

GilLock* ThreadLocks::get( const std::thread::id& key ) {
    std::hash< std::thread::id > hasher;
    size_t hashed_key = hasher( key );
    GilLock* ret = NULL;
    _read_write_lock.read_lock();
    if ( _locks.find( hashed_key ) != _locks.end() ) {
        ret = _locks[ hashed_key ];
    }
    _read_write_lock.read_unlock();
    return ret;
}

void ThreadLocks::remove( const std::thread::id& key ) {
    std::hash< std::thread::id > hasher;
    size_t hashed_key = hasher( key );
    _read_write_lock.write_lock();
    if ( _locks.find( hashed_key ) != _locks.end() ) {
        _locks.erase( hashed_key );
    }
    _read_write_lock.write_unlock();
}
