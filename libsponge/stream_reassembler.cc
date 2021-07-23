#include "stream_reassembler.hh"
#include <iostream>

// Dummy implementation of a stream reassembler.

// For Lab 1, please replace with a real implementation that passes the
// automated checks run by `make check_lab1`.

// You will need to add private members to the class declaration in `stream_reassembler.hh`

// template <typename... Targs>
// void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

StreamReassembler::StreamReassembler(const size_t capacity) : _output(capacity), _capacity(capacity)
, to_assume_data(capacity), assume_finished(capacity) , next_conti(0), _eof(false), to_assume_data_size(capacity), cur_max(0) {
    for(size_t i = 0; i < to_assume_data_size; i++) {
        assume_finished[i] = true;
    }
}

//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {
    // DUMMY_CODE(data, index, eof);

    if(_eof && next_conti >= cur_max) {
        return;
    }

    size_t len = data.size();
    size_t rec_max = next_conti + to_assume_data_size;
    if(index + data.size() > rec_max){
        len -= (index + data.size() - rec_max);
    }

    if (eof){
        _eof = eof;
    }

    cur_max = index + len > cur_max ? index + len : cur_max;

    for(size_t i = index; i < index + len; i++) {
        if (i < next_conti){
            continue;
        }
        to_assume_data[i % to_assume_data_size] = data[i - index];
        assume_finished[i % to_assume_data_size] = false;
        
    }

    string write_to_output = "";
    size_t re = _output.remaining_capacity();
    while (re-- && !assume_finished[next_conti % to_assume_data_size]) {
        write_to_output += to_assume_data[next_conti % to_assume_data_size];
        assume_finished[next_conti % to_assume_data_size] = true;
        next_conti++;
    }
    
    _output.write(write_to_output);



    if (_eof && next_conti >= cur_max){
        _output.end_input();
    }
    
}

size_t StreamReassembler::unassembled_bytes() const { 
    size_t res = 0;
    for(size_t i = 0; i < assume_finished.size(); i++) {
        if (assume_finished[i] == false){
            res++;
        }
    }
    return res;
}

bool StreamReassembler::empty() const {
    bool res = true;
    for(size_t i = 0; i < assume_finished.size(); i++) {
        if (assume_finished[i] == false){
            res = false;
        }
    }
    return res;
}
