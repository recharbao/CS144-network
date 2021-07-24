#include "wrapping_integers.hh"

// Dummy implementation of a 32-bit wrapping integer

// For Lab 2, please replace with a real implementation that passes the
// automated checks run by `make check_lab2`.

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

//! Transform an "absolute" 64-bit sequence number (zero-indexed) into a WrappingInt32
//! \param n The input absolute 64-bit sequence number
//! \param isn The initial sequence number
WrappingInt32 wrap(uint64_t n, WrappingInt32 isn) {
    // DUMMY_CODE(n, isn);
    uint32_t v = static_cast<uint32_t>((static_cast<uint64_t>(isn.raw_value()) + n) % (1ul << 32));
    return WrappingInt32{v};
}

//! Transform a WrappingInt32 into an "absolute" 64-bit sequence number (zero-indexed)
//! \param n The relative sequence number
//! \param isn The initial sequence number
//! \param checkpoint A recent absolute 64-bit sequence number
//! \returns the 64-bit sequence number that wraps to `n` and is closest to `checkpoint`
//!
//! \note Each of the two streams of the TCP connection has its own ISN. One stream
//! runs from the local TCPSender to the remote TCPReceiver and has one ISN,
//! and the other stream runs from the remote TCPSender to the local TCPReceiver and
//! has a different ISN.
uint64_t unwrap(WrappingInt32 n, WrappingInt32 isn, uint64_t checkpoint) {
    // DUMMY_CODE(n, isn, checkpoint);


    // a little tricky    
    uint64_t _n = static_cast<uint64_t>(n - isn);
    uint64_t s = _n | (checkpoint & (~((1ul << 32) - 1)));

    if (s <= checkpoint){
        if(s + (1ul << 32) - checkpoint > checkpoint - s) {
            return s;
        }else{
            return s + (1ul << 32);
        }
    }else{
        if (s < (1ul << 32) || checkpoint - (s - (1ul << 32)) > s - checkpoint){
            return s;
        }else{
            return s - (1ul << 32);
        }
    }


    // uint64_t _n = n.raw_value();
    // uint64_t _isn = isn.raw_value();
    // uint64_t ts_chec = checkpoint / (1ul << 31);
    // uint64_t ts_n = _n / (1ul << 31);

    

    // if (ts_chec > ts_n) {
    //     uint64_t a[3] = {_n * (1ul << (ts_chec - ts_n - 1)) + _isn, _n * (1ul << (ts_chec - ts_n)) + _isn, _n * (1ul << (ts_chec - ts_n + 1)) + _isn};
    //     if (a[1] < checkpoint) {
    //         if(a[2] - checkpoint > checkpoint - a[1]) {
    //             return a[1];
    //         }else{
    //             return a[2];
    //         }
    //     }else {
    //         if (a[1] - checkpoint > checkpoint - a[0]){
    //             return a[0];
    //         }else {
    //             return a[1];
    //         }
    //     }

    // }else if(ts_chec < ts_n){
    //     uint64_t a[3] = {_n * (1ul << (ts_n - ts_chec - 1)) + _isn, _n * (1ul << (ts_n - ts_chec)) + _isn, _n * (1ul << (ts_n - ts_chec + 1)) + _isn};
    
    //     if (a[1] < checkpoint) {
    //         if(a[2] - checkpoint > checkpoint - a[1]) {
    //             return a[1];
    //         }else{
    //             return a[2];
    //         }
    //     }else {
    //         if (a[1] - checkpoint > checkpoint - a[0]){
    //             return a[0];
    //         }else {
    //             return a[1];
    //         }
    //     }
    // }else if (ts_chec < (1ul << 31)) {

    //     uint64_t a[2] = {_n * (1ul << (ts_n - ts_chec)) + _isn, _n * (1ul << (ts_n - ts_chec + 1)) + _isn};

    //     if(a[1] - checkpoint > checkpoint - a[0]) {
    //         return a[0];
    //     }else{
    //         return a[1];
    //     }

    // }else{
    //     uint64_t a[3] = {_n * (1ul << (ts_n - ts_chec - 1)) + _isn, _n * (1ul << (ts_n - ts_chec)) + _isn, _n * (1ul << (ts_n - ts_chec + 1)) + _isn};
    
    //     if (a[1] < checkpoint) {
    //         if(a[2] - checkpoint > checkpoint - a[1]) {
    //             return a[1];
    //         }else{
    //             return a[2];
    //         }
    //     }else {
    //         if (a[1] - checkpoint > checkpoint - a[0]){
    //             return a[0];
    //         }else {
    //             return a[1];
    //         }
    //     }
    // }
}
