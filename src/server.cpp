//compile command: g++ server.cpp socket.cpp http.hpp -o server -l ws2_32
#include "server.hpp"

int main(int argc, char *argv[]) {
    std::cout << "Starting Server\n";

    struct ip ip_info;
    get_ip_info(ip_info, argc, argv);

    SOCKET server{INVALID_SOCKET};
    socket_state response = init_socket(server, ip_info);
    if (response != socket_state::PASS) return response;

    response_info req{
        DEFAULT_BUFFLEN,
        (argc > 1) ? argv[terminal_codes::root_level] : (const char *)DEFAULT_ROOT
    };

    response = handle_requests(server, req);
    return response;
}

void get_ip_info(ip &ip_info, int argc, char *argv[]) {
    switch (argc - terminal_codes::root_level) {
        default:
        case terminal_codes::listener_level:
            ip_info.listen_count = atoi(argv[terminal_codes::listener_level]);
            __attribute__ ((fallthrough));

        case terminal_codes::port_level:
            ip_info.port = atoi(argv[terminal_codes::port_level]);
            ip_info.port_s = argv[terminal_codes::port_level];
            __attribute__ ((fallthrough));

        case terminal_codes::ip_level:
            ip_info.ipaddr = argv[terminal_codes::ip_level];
            __attribute__ ((fallthrough));

        case terminal_codes::root_level:
        case terminal_codes::default_level:
            break;
    };

    switch (argc - terminal_codes::root_level) {
        case terminal_codes::default_level:
        case terminal_codes::root_level:
            ip_info.ipaddr = DEFAULT_IP;
            __attribute__ ((fallthrough));

        case terminal_codes::ip_level:
            ip_info.port = DEFAULT_PORT;
            ip_info.port_s = (const char *)DEFAULT_PORT_S;
            __attribute__ ((fallthrough));
            
        case terminal_codes::port_level:
            ip_info.listen_count = DEFAULT_LISTENER_COUNT;
            __attribute__ ((fallthrough));

        default: break;
    };

}

