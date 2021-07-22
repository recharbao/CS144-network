#include "byte_stream.hh"

// Dummy implementation of a flow-controlled in-memory byte stream.

// For Lab 0, please replace with a real implementation that passes the
// automated checks run by `make check_lab0`.

// You will need to add private members to the class declaration in `byte_stream.hh`

// template <typename... Targs>
// void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

ByteStream::ByteStream(const size_t capacity) : _capacity(capacity), _used(0), _tol_read(0), _tol_write(0), _end(false) {}

size_t ByteStream::write(const string &data) {
    size_t writen = 0;
    size_t len = data.size();
    

    if (len <= _capacity - _used){
        writen = len;
        buffer += data;
        _used += len;
    }else {
        len = _capacity - _used;
        writen = len;
        buffer += data.substr(0, len);
        _used += len;
    }
    
    _tol_write += writen;

    return writen;
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {
    
    string res = "";
    res = buffer.substr(0, len);
    return res;
}

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) {
    _used -= len;
    _tol_read += len;
    buffer.erase(0, len);
}

//! Read (i.e., copy and then pop) the next "len" bytes of the stream
//! \param[in] len bytes will be popped and returned
//! \returns a string
std::string ByteStream::read(const size_t len) {
    string res;
    size_t len_ = len;
    len_ = len_ > _used ? _used : len_;
    res = peek_output(len_);
    pop_output(len_);
    
    return res;
}

void ByteStream::end_input() {_end = true; }

bool ByteStream::input_ended() const { return _end; }

size_t ByteStream::buffer_size() const { return _used; }

bool ByteStream::buffer_empty() const { return _used == 0; }

bool ByteStream::eof() const { return buffer.size() == 0 && _end ? true : false; }

size_t ByteStream::bytes_written() const { return _tol_write; }

size_t ByteStream::bytes_read() const { return _tol_read; }

size_t ByteStream::remaining_capacity() const { return _capacity - _used; }
