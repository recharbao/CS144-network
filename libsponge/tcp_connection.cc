#include "tcp_connection.hh"

#include <iostream>

// Dummy implementation of a TCP connection

// For Lab 4, please replace with a real implementation that passes the
// automated checks run by `make check`.

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

size_t TCPConnection::remaining_outbound_capacity() const { return {}; }

size_t TCPConnection::bytes_in_flight() const { return {}; }

size_t TCPConnection::unassembled_bytes() const { return {}; }

size_t TCPConnection::time_since_last_segment_received() const { return {}; }

void TCPConnection::segment_received(const TCPSegment &seg) { DUMMY_CODE(seg); }

bool TCPConnection::active() const { return {}; }

size_t TCPConnection::write(const string &data) {
    DUMMY_CODE(data);
    return {};
}

//! \param[in] ms_since_last_tick number of milliseconds since the last call to this method
void TCPConnection::tick(const size_t ms_since_last_tick) { DUMMY_CODE(ms_since_last_tick); }

void TCPConnection::end_input_stream() {}

void TCPConnection::connect() {}

TCPConnection::~TCPConnection() {
    try {
        if (active()) {
            cerr << "Warning: Unclean shutdown of TCPConnection\n";

            // Your code here: need to send a RST segment to the peer
        }
    } catch (const exception &e) {
        std::cerr << "Exception destructing TCP FSM: " << e.what() << std::endl;
    }
}



// #include "tcp_connection.hh"

// #include <iostream>

// // Dummy implementation of a TCP connection

// // For Lab 4, please replace with a real implementation that passes the
// // automated checks run by `make check`.

// // template <typename... Targs>
// // void DUMMY_CODE(Targs &&... /* unused */) {}

// using namespace std;

// size_t TCPConnection::remaining_outbound_capacity() const { return _receiver.stream_out().remaining_capacity(); }

// size_t TCPConnection::bytes_in_flight() const { return _sender.bytes_in_flight(); }

// size_t TCPConnection::unassembled_bytes() const { return _receiver.unassembled_bytes(); }

// size_t TCPConnection::time_since_last_segment_received() const { return _cur_time - _last_time; }

// void TCPConnection::segment_received(const TCPSegment &seg) { // DUMMY_CODE(seg);
//     _last_time = _cur_time;

//     bool flag = false;

//     if (seg.header().rst){
//         _sender.stream_in().set_error();
//         _receiver.stream_out().set_error();
//     }

    


    
//     if (seg.header().ack && (_receiver.ackno().has_value() || seg.header().syn)){
//         bool ack_succ = _sender.ack_received(seg.header().ackno, seg.header().win);
//         if (!ack_succ){
//             flag = true;
//         }else {
//             _sender.fill_window();
//         }
//     }

//     bool rec_succ = _receiver.segment_received(seg);
//     if (!_syn && seg.header().syn){
//         connect();
//     }

//     if (rec_succ && _segments_out.empty() && seg.length_in_sequence_space()) {
//         flag = true;
//     }

//     if (flag){
//         _sender.send_empty_segment();
//     }
    
//     segment_enqueue();

//     _sender.fill_window();
//     segment_enqueue();
// }

// bool TCPConnection::active() const {
//     return _linger_after_streams_finish;
// }

// size_t TCPConnection::write(const string &data) {
//     // DUMMY_CODE(data);
//     // return {};

//     size_t n = _sender.stream_in().write(data);

//     _sender.fill_window();
//     segment_enqueue();

//     return n;
// }

// //! \param[in] ms_since_last_tick number of milliseconds since the last call to this method
// void TCPConnection::tick(const size_t ms_since_last_tick) { // DUMMY_CODE(ms_since_last_tick); 
//     _cur_time += ms_since_last_tick;
//     _sender.tick(ms_since_last_tick);
//     segment_enqueue();
// }

// void TCPConnection::end_input_stream() {
//     _sender.stream_in().end_input();
//     _sender.fill_window();
//     segment_enqueue();
// }

// void TCPConnection::connect() {
//     _sender.fill_window();
//     segment_enqueue();
// }


// void TCPConnection::segment_enqueue() {
//     while(_sender.segments_out().empty()){        
//         _segments_out.push(_sender.segments_out().front());
//         _sender.segments_out().pop();
//     }
    
// }

// TCPConnection::~TCPConnection() {
//     try {
//         if (active()) {
//             cerr << "Warning: Unclean shutdown of TCPConnection\n";

//             _linger_after_streams_finish = true;

//             // Your code here: need to send a RST segment to the peer
//         }
//     } catch (const exception &e) {
//         std::cerr << "Exception destructing TCP FSM: " << e.what() << std::endl;
//     }
// }