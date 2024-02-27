#ifndef UTILS_HPP
#define UTILS_HPP

// #define INCLUDE_WINDOWS
#ifdef WIN32
    #ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
    #endif
    #include <windows.h>
#endif

#include <winsock2.h>
#include <ws2tcpip.h>
#include <bits/stdc++.h>
#include <string>
#include <unordered_map>
#include <iostream>
#include <thread>
#include <future>
#include <mutex>

#define DEFAULT_PORT 80
#define DEFAULT_PORT_S "80"
#define DEFAULT_IP "127.0.0.1"
#define DEFAULT_BUFFLEN 32000
#define MAX_BUFFLEN 64000
#define DEFAULT_LISTENER_COUNT 100
#define DEFAULT_ROOT "..\\home"
#define HTTP_TEMPLATE "HTTP/%d.%d %d %s\n"
#define BUFF_PADDING 100

enum request_type {
    GET,
    HEAD, 
    POST, 
    PUT, 
    PATCH, 
    DELETE_, 
    CONNECT, 
    OPTIONS, 
    TRACE, 
};

enum terminal_codes {
    default_level,
    root_level,
    ip_level,
    port_level,
    listener_level,
};

enum socket_state {
    PASS,
    STARTUP_FAIL, 
    GETADDR_FAIL,
    SOCKET_FAIL,
    BIND_FAIL, 
    LISTEN_FAIL, 
    ACCEPT_FAIL, 
    SHUTDOWN_FAIL, 
    SEND_FAIL,
    RECV_FAIL,
    CLOSE,
};

struct ip_info {
    int port;
    const char *port_s;
    const char *ipaddr;
    int listen_count;
};

typedef socket_state state;
typedef std::string dir;
#endif