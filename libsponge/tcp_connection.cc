#include "tcp_connection.hh"

#include <iostream>

// Dummy implementation of a TCP connection

// For Lab 4, please replace with a real implementation that passes the
// automated checks run by `make check`.

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

size_t TCPConnection::remaining_outbound_capacity() const { return _sender.stream_in().remaining_capacity(); }

size_t TCPConnection::bytes_in_flight() const { return _sender.bytes_in_flight(); }

size_t TCPConnection::unassembled_bytes() const { return _receiver.unassembled_bytes(); }

size_t TCPConnection::time_since_last_segment_received() const { return _cur_time - _rec_time; }

//! \param[in] ms_since_last_tick number of milliseconds since the last call to this method
void TCPConnection::tick(const size_t ms_since_last_tick) {
    _cur_time += ms_since_last_tick;
    _sender.tick(ms_since_last_tick);

    if (time_since_last_segment_received() >= (10 * _cfg.rt_timeout)) {
        _linger_after_streams_finish = false;
    }

    if (_sender.consecutive_retransmissions() > _cfg.MAX_RETX_ATTEMPTS) {
        _sender.stream_in().set_error();
        _receiver.stream_out().set_error();
        TCPSegment seg;
        seg.header().rst = true;
        _sender.send_empty_segment(seg);
    }
}

void TCPConnection::segment_received(const TCPSegment &seg) {
    _rec_time = _cur_time;
    TCPHeader header = seg.header();
    if (header.rst) {
        _sender.stream_in().set_error();
        _receiver.stream_out().set_error();
        return;
    }

    if (header.ack) {
        _sender.ack_received(header.ackno, header.win);
    }

    _receiver.segment_received(seg);
   

    if (_receiver.stream_out().input_ended() && !_sender.stream_in().eof()) {
        _linger_after_streams_finish = false;
    }


    // TCPSegment seg_ack;
    // seg_ack.header().ack = true;
    // seg_ack.header().ackno = _receiver.ackno().value();
    // _sender.send_empty_segment(seg_ack);
    
    _sender.fill_window();
   

    if (seg.length_in_sequence_space() > 0 && _sender.segments_out().empty()) {
        _sender.send_empty_segment();
        TCPSegment seg_ack = _sender.segments_out().front();
        if (_receiver.ackno().has_value()) {
            seg_ack.header().ack = true;
            seg_ack.header().ackno = _receiver.ackno().value();
        }

        seg_ack.header().win = static_cast<uint16_t>(_receiver.window_size());

        _segments_out.push(seg_ack);

        _sender.segments_out().pop();
    }

    while (!_sender.segments_out().empty()) {
        TCPSegment seg1 = _sender.segments_out().front();
        
        if (_receiver.ackno().has_value()) {
            seg1.header().ack = true;
            seg1.header().ackno = _receiver.ackno().value();
        }

        seg1.header().win = static_cast<uint16_t>(_receiver.window_size());
        _segments_out.push(seg1);
        _sender.segments_out().pop();
    }
}

bool TCPConnection::active() const {
    if ((_receiver.stream_out().input_ended() && unassembled_bytes() == 0) && 
    (_sender.stream_in().input_ended() && remaining_outbound_capacity() == 0)
     && (!_linger_after_streams_finish) && (bytes_in_flight() == 0)) {
        return false;
    }
    return true;
}

void TCPConnection::connect() {
    // syn
    _sender.fill_window();
    while (!_sender.segments_out().empty()) {
        TCPSegment seg1 = _sender.segments_out().front();

        // WrappingInt32 tmp{0};

        if (_receiver.ackno().has_value()) {
            seg1.header().ack = true;
            seg1.header().ackno = _receiver.ackno().value();
        }

        seg1.header().win = static_cast<uint16_t>(_receiver.window_size());
        _segments_out.push(seg1);
        _sender.segments_out().pop();
    }
}

size_t TCPConnection::write(const string &data) {
    size_t bytesNum =  _sender.stream_in().write(data);
    // DUMMY_CODE(data);
    _sender.fill_window();
    return bytesNum;
}


void TCPConnection::end_input_stream() {_sender.stream_in().end_input();}

TCPConnection::~TCPConnection() {
    try {
        if (active()) {
            cerr << "Warning: Unclean shutdown of TCPConnection\n";
            // Your code here: need to send a RST segment to the peer
            _sender.stream_in().set_error();
            _receiver.stream_out().set_error();

            TCPSegment seg;
            seg.header().rst = true;
            _sender.send_empty_segment(seg);
        }
    } catch (const exception &e) {
        std::cerr << "Exception destructing TCP FSM: " << e.what() << std::endl;
    }
}
