#ifndef SPONGE_LIBSPONGE_TIMER_HH
#define SPONGE_LIBSPONGE_TIMER_HH

typedef unsigned int size_tt; 

class tcp_timer
{
private:
    size_tt _timeout = 0;
    size_tt _elapsed_time = 0;
    size_tt _expire_time = 0;

public:

    tcp_timer(size_tt init_timeout);
    
    void set_timeout(size_tt timeout);
    // void set_expire_time(size_t expire_time);
    void update_elapsed_time(size_tt ms_since_last_tick);
    bool out_of_time(); //true out of time, false are not
};


#endif // SPONGE_LIBSPONGE_TCP_RECEIVER_HH
