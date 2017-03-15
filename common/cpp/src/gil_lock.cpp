#include "gil_lock.hpp"

using namespace std;
using namespace Common;

GilLock::GilLock()
: state( NULL ) {
    ThreadLocks::add( this_thread::get_id(), this );
}

void GilLock::unlock() {
    if ( !state ) {
        state = PyEval_SaveThread();
    }
}

void GilLock::lock() {
    if ( state ) {
        PyEval_RestoreThread( state );
        state = NULL;
    }
}

GilLock::~GilLock() {
    lock();
    ThreadLocks::remove( this_thread::get_id() );
}
