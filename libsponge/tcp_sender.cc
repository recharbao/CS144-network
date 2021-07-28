#include "tcp_sender.hh"

#include "tcp_config.hh"

#include <random>
#include<math.h>
#include <iostream>

// Dummy implementation of a TCP sender

// For Lab 3, please replace with a real implementation that passes the
// automated checks run by `make check_lab3`.

// template <typename... Targs>
// void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

//! \param[in] capacity the capacity of the outgoing byte stream
//! \param[in] retx_timeout the initial amount of time to wait before retransmitting the oldest outstanding segment
//! \param[in] fixed_isn the Initial Sequence Number to use, if set (otherwise uses a random ISN)
TCPSender::TCPSender(const size_t capacity, const uint16_t retx_timeout, const std::optional<WrappingInt32> fixed_isn)
    : _isn(fixed_isn.value_or(WrappingInt32{random_device()()}))
    , _initial_retransmission_timeout{retx_timeout}
    , _stream(capacity) {}

uint64_t TCPSender::bytes_in_flight() const {
    return _bytes_in_flight;
}

void TCPSender::fill_window() {
    while (1) {
        uint64_t segment_payload_size = TCPConfig::MAX_PAYLOAD_SIZE < _window_size ? TCPConfig::MAX_PAYLOAD_SIZE : _window_size;

        TCPSegment seg;

        if (_next_seqno >= _fin_ackno){
            return;
        }
        
        seg.payload() = Buffer(_stream.read(segment_payload_size)); // &&
        seg.header().seqno = next_seqno();


        if (_next_seqno == 0){
            seg.header().syn = true;
            retransmissions_nums = 0;
            _timer.set_timeout(_initial_retransmission_timeout);
        }    
        
        
        if (_stream.eof() && _window_size > seg.payload().size()){
            seg.header().fin = true;
            _fin_ackno = _next_seqno + seg.length_in_sequence_space();
        }

        _next_seqno += seg.length_in_sequence_space();
        _bytes_in_flight = _next_seqno - _ackno_abs;
        _window_size -= seg.payload().size();
        _checkpoint = _next_seqno;
        

        if (!seg.length_in_sequence_space()){
            return;
        }
        
        _segments_out.push(seg);
        _segment_outstanding.push(seg);
        
    }
}

//! \param ackno The remote receiver's ackno (acknowledgment number)
//! \param window_size The remote receiver's advertised window size
void TCPSender::ack_received(const WrappingInt32 ackno, const uint16_t window_size) { // DUMMY_CODE(ackno, window_size); 
    _ackno = ackno;
    
    if(unwrap(_ackno, _isn, _checkpoint) < _ackno_abs) {
        return;
    }

    _ackno_abs = unwrap(_ackno, _isn, _checkpoint);
    
    _bytes_in_flight = _next_seqno - _ackno_abs;
    
    if(window_size - _bytes_in_flight < 1 && _ackno_abs == _next_seqno) {
        _window_size = 1;
        _window_size_fix = true;
    }else {
        _window_size = window_size - _bytes_in_flight;
        _window_size_fix = false;
    }
    
    
    uint64_t top_ackno_abs = unwrap(_segment_outstanding.front().header().seqno, _isn, _checkpoint);

    if (top_ackno_abs + _segment_outstanding.front().length_in_sequence_space() > _ackno_abs) {
        return;
    }

    while (!_segment_outstanding.empty() && _ackno_abs > top_ackno_abs){
        _segment_outstanding.pop();
        top_ackno_abs = unwrap(_segment_outstanding.front().header().seqno, _isn, _checkpoint);
    }

    retransmissions_nums = 0;
    _timer.set_timeout(_initial_retransmission_timeout);

    // cout << "_bytes_in_flight = " << _bytes_in_flight << endl;
}

//! \param[in] ms_since_last_tick the number of milliseconds since the last call to this method
void TCPSender::tick(const size_t ms_since_last_tick) { // DUMMY_CODE(ms_since_last_tick); 
    _timer.update_elapsed_time(ms_since_last_tick);
    if (_timer.out_of_time()){

        if(!_segment_outstanding.empty()){
            // cout << "=====================================================" << endl;
            TCPSegment tmp_outstanding = _segment_outstanding.front();
            _segments_out.push(tmp_outstanding);
            // _segment_outstanding.pop();
            if (!_window_size_fix){
                retransmissions_nums++;
            }
        }

        _timer.set_timeout(_initial_retransmission_timeout * pow(2, retransmissions_nums));
    }
}

unsigned int TCPSender::consecutive_retransmissions() const {
    return retransmissions_nums;
}

void TCPSender::send_empty_segment() {
    TCPSegment seg;
    seg.header().seqno = next_seqno();
    _segments_out.push(seg);
}
