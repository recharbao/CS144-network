#include "network_interface.hh"

#include "arp_message.hh"
#include "ethernet_frame.hh"

#include <iostream>

// Dummy implementation of a network interface
// Translates from {IP datagram, next hop address} to link-layer frame, and from link-layer frame to IP datagram

// For Lab 5, please replace with a real implementation that passes the
// automated checks run by `make check_lab5`.

// You will need to add private members to the class declaration in `network_interface.hh`

// template <typename... Targs>
// void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

//! \param[in] ethernet_address Ethernet (what ARP calls "hardware") address of the interface
//! \param[in] ip_address IP (what ARP calls "protocol") address of the interface
NetworkInterface::NetworkInterface(const EthernetAddress &ethernet_address, const Address &ip_address)
    : _ethernet_address(ethernet_address), _ip_address(ip_address) {
    cerr << "DEBUG: Network interface has Ethernet address " << to_string(_ethernet_address) << " and IP address "
         << ip_address.ip() << "\n";
}

//! \param[in] dgram the IPv4 datagram to be sent
//! \param[in] next_hop the IP address of the interface to send it to (typically a router or default gateway, but may also be another host if directly connected to the same network as the destination)
//! (Note: the Address type can be converted to a uint32_t (raw 32-bit IP address) with the Address::ipv4_numeric() method.)
void NetworkInterface::send_datagram(const InternetDatagram &dgram, const Address &next_hop) {
    // convert IP address of next hop to raw 32-bit representation (used in ARP header)
    const uint32_t next_hop_ip = next_hop.ipv4_numeric();
    auto iterator = _ip_map_ethernet.find(next_hop_ip);
    if (iterator == _ip_map_ethernet.end()) {
        // ARP
        EthernetFrame e;
        e.header().type = EthernetHeader::TYPE_ARP;
        EthernetAddress

    } else {
        // IP
        EthernetFrame e;
        e.header().src = _ethernet_address;
        e.header().dst = iterator->second;
        e.header().type = EthernetHeader::TYPE_IPv4;
        e.payload() = dgram.serialize();
        _frames_out.push(e);
    }

    // DUMMY_CODE(dgram, next_hop, next_hop_ip);
}

//! \param[in] frame the incoming Ethernet frame
optional<InternetDatagram> NetworkInterface::recv_frame(const EthernetFrame &frame) {
    // DUMMY_CODE(frame);

    InternetDatagram idata;
    if (frame.header().type == EthernetHeader::TYPE_IPv4) {
        // ip
        if (idata.parse(frame.payload()) == ParseResult::NoError) {
            // 如果没有立马加入
            EthernetAddress ed = frame.header().src;
            idata.parse(frame.payload());
            uint32_t src = idata.header().src;
            _ip_map_ethernet[src] = ed;
            return idata;
        }
    } else if (frame.header().type == EthernetHeader::TYPE_ARP) {
        EthernetAddress ed = frame.header().src;
        idata.parse(frame.payload());
        uint32_t src = idata.header().src;
        _ip_map_ethernet[src] = ed;
        // reply
        InternetDatagram idata2;
        idata2.header().src = _ip_address.ip();
        idata2.header().dst = src;

        send_datagram(idata2,Address::from_ipv4_numeric(src));
    }

    return {};
}

//! \param[in] ms_since_last_tick the number of milliseconds since the last call to this method
void NetworkInterface::tick(const size_t ms_since_last_tick) {
    
}
