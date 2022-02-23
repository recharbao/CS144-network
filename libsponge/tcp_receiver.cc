#include "tcp_receiver.hh"
#include <iostream>

using namespace std;

// Dummy implementation of a TCP receiver

// For Lab 2, please replace with a real implementation that passes the
// automated checks run by `make check_lab2`.

// template <typename... Targs>
// void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

void TCPReceiver::segment_received(const TCPSegment &seg) {
    // DUMMY_CODE(seg);
    WrappingInt32 seqno = seg.header().seqno;

    if ((_syn && seg.header().syn) || (!_syn && !seg.header().syn) || (_end_rev)){
        return;
    }
    
    if(seg.header().syn) {
        _syn = true;
        _isn = seg.header().seqno;
    }

    uint64_t seqno_abs = unwrap(seqno, _isn, _checkpoint);
    
    if (seg.header().fin) {
        _fin = true;
        _end_seqno = seqno_abs + seg.length_in_sequence_space() - 2;
    }

    if (seqno_abs == 0 && seg.payload().size() != 0){
        _bis = 0;
    }
    
    uint64_t index = seqno_abs - _bis;

    string content;
    
    if (seqno_abs + seg.length_in_sequence_space() <= _ackno_abs || seqno_abs >= _ackno_abs + stream_out().buffer_capacity()) {
        return;
    }
    
    content = seg.payload().copy().substr(0, min(seg.payload().size(), window_size()));
    

    _reassembler.push_substring(content, index, _fin);

    _checkpoint = stream_out().bytes_written();

    _ackno_abs = stream_out().bytes_written() + 1;

    if(_fin && _ackno_abs >= _end_seqno) {
        _reassembler.stream_out().end_input();
        _ackno_abs++;
        _end_rev = true;
    }

    _ackno = wrap(_ackno_abs, _isn);
}

optional<WrappingInt32> TCPReceiver::ackno() const {
    if (_syn){
        return _ackno;
    }
    
    return {};
}

size_t TCPReceiver::window_size() const { return stream_out().remaining_capacity(); }
