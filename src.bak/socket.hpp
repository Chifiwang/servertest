#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <iostream>
#include "utils.hpp"
#include "http.hpp"

socket_state init_socket(SOCKET &soc, const ip &ip);
socket_state handle_requests(SOCKET &server, response_info &req);

#endif