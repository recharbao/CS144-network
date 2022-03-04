#include "router.hh"

#include <iostream>

using namespace std;

// Dummy implementation of an IP router

// Given an incoming Internet datagram, the router decides
// (1) which interface to send it out on, and
// (2) what next hop address to send it to.

// For Lab 6, please replace with a real implementation that passes the
// automated checks run by `make check_lab6`.

// You will need to add private members to the class declaration in `router.hh`

// template <typename... Targs>
// void DUMMY_CODE(Targs &&... /* unused */) {}

//! \param[in] route_prefix The "up-to-32-bit" IPv4 address prefix to match the datagram's destination address against
//! \param[in] prefix_length For this route to be applicable, how many high-order (most-significant) bits of the route_prefix will need to match the corresponding bits of the datagram's destination address?
//! \param[in] next_hop The IP address of the next hop. Will be empty if the network is directly attached to the router (in which case, the next hop address should be the datagram's final destination).
//! \param[in] interface_num The index of the interface to send the datagram out on.
void Router::add_route(const uint32_t route_prefix,
                       const uint8_t prefix_length,
                       const optional<Address> next_hop,
                       const size_t interface_num) {
    cerr << "DEBUG: adding route " << Address::from_ipv4_numeric(route_prefix).ip() << "/" << int(prefix_length)
         << " => " << (next_hop.has_value() ? next_hop->ip() : "(direct)") << " on interface " << interface_num << "\n";

    // DUMMY_CODE(route_prefix, prefix_length, next_hop, interface_num);
    // Your code here.
    auto _prefix_length_map_router_deep_map = _prefix_length_map_router_prefix_deep_map_next_hop.find(prefix_length);
    if (_prefix_length_map_router_deep_map == _prefix_length_map_router_prefix_deep_map_next_hop.end()) {
        if (next_hop.has_value()) {
            std::map<uint32_t, Address> _router_prefix_deep_map_next_hop;
           _router_prefix_deep_map_next_hop[route_prefix] = next_hop;
           _prefix_length_map_router_prefix_deep_map_next_hop[prefix_length] = _router_prefix_deep_map_next_hop;
        }
    } else {
        std::map<uint32_t, Address> _router_prefix_deep_map_next_hop = _prefix_length_map_router_deep_map->second;
        _router_prefix_deep_map_next_hop[route_prefix] = next_hop; 
    }

    _next_hop_map_interface[next_hop->ipv4_numeric()] = interface_num;
}

//! \param[in] dgram The datagram to be routed
void Router::route_one_datagram(InternetDatagram &dgram) {
    // Your code here.
    auto header = &dgram.header();
    uint32_t dst = header->dst;
    size_t interface_index;
    Address *next_hop;
    for (auto it1 : _prefix_length_map_router_prefix_deep_map_next_hop) {
        uint8_t len = it1.first;
        uint32_t match = ~ ((1 << len) - 1);
        for (auto it2 : it1.second) {
            if ((match & dst) == it2.first) {
                next_hop = &it2.second;
            }
        }
    }

    interface_index = _next_hop_map_interface.find(next_hop->ipv4_numeric())->second;
    auto inf = interface(interface_index);
    inf.send_datagram(dgram, *next_hop);
}

void Router::route() {
    // Go through all the interfaces, and route every incoming datagram to its proper outgoing interface.
    for (auto &interface : _interfaces) {
        auto &queue = interface.datagrams_out();
        while (not queue.empty()) {
            route_one_datagram(queue.front());
            queue.pop();
        }
    }
}
