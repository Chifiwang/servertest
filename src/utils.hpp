#ifndef UTILS_HPP
#define UTILS_HPP

#define INCLUDE_WINDOWS
#ifdef INCLUDE_WINDOWS
    #ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
    #endif
    #include <windows.h>
#endif

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <bits/stdc++.h>

#define DEFAULT_PORT 80
#define DEFAULT_PORT_S "80"
#define DEFAULT_IP "127.0.0.1"
#define DEFAULT_BUFFLEN 32000
#define MAX_BUFFLEN 64000
#define DEFAULT_LISTENER_COUNT 100
#define DEFAULT_ROOT "..\\home"
#define HTTP_TEMPLATE "HTTP/%d.%d %d OK\n"
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
};

typedef std::unordered_map<std::string, std::string> header_fields;
struct request_info {
// info buffer
    char buff[DEFAULT_BUFFLEN];
// request line
    request_type type;
    std::string uri;
    std::string extention;
    int maj_ver;
    int min_ver;
// header_fields
    header_fields headers;
// body
    std::string body;
};

struct response_info {
    // const int buff_len;
    const char *root_dir;
    int response_type;
    int maj_ver;
    int min_ver;

    header_fields headers;
// do something with this in the future
    std::string body;

    response_info(const char *root) : root_dir{root} {}
};

struct ip {
    int port;
    const char *port_s;
    const char *ipaddr;
    int listen_count;
};

#endif