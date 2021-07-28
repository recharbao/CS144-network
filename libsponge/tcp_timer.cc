#include "tcp_timer.hh"

tcp_timer::tcp_timer(size_tt init_timeout) {
    _timeout = init_timeout;
}

bool tcp_timer::out_of_time() {
    if(_elapsed_time >= _expire_time) {
        return true;
    }else {
        return false;
    }
}

void tcp_timer::update_elapsed_time(size_tt ms_since_last_tick) {
    _elapsed_time += ms_since_last_tick;
}

void tcp_timer::set_timeout(size_tt timeout) {
    _timeout = timeout;
    _expire_time = _elapsed_time + _timeout;
}



