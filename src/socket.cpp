#include "socket.hpp"

socket_state init_socket(SOCKET &soc, const ip& ip) {
    WSAData wsa_data;
    int return_code;
    struct addrinfo *result = NULL, hints;
    struct sockaddr_in localaddr;
    int localaddr_size;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    ZeroMemory(&localaddr, sizeof(localaddr));
    localaddr.sin_family = AF_INET;
    localaddr.sin_addr.s_addr = inet_addr(ip.ipaddr);
    localaddr.sin_port = htons(ip.port);

    localaddr_size = sizeof(localaddr);

    return_code = WSAStartup(MAKEWORD(1, 1), &wsa_data);
    if (return_code) {
        std::cerr << "WSAStartup() failed with code: " << return_code;
        return socket_state::STARTUP_FAIL;
    } else std::cout << "WSAStartup() successful\n";

    return_code = getaddrinfo(NULL, ip.port_s, &hints, &result);
    if (return_code) {
        std::cerr << "getaddrinfo() failed with code: " << return_code;
        WSACleanup();
        return socket_state::GETADDR_FAIL;
    } else std::cout << "Address info obtained\n";

    soc = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (soc == INVALID_SOCKET) {
        std::cerr << "socket() failed with error: " << WSAGetLastError();
        freeaddrinfo(result);
        WSACleanup();
        return socket_state::SOCKET_FAIL;
    } else std::cout << "Socket initialized\n";

    return_code = bind(soc, (SOCKADDR *) &localaddr, localaddr_size);
    if (return_code == SOCKET_ERROR) {
        std::cerr << "bind() failed with error: " << WSAGetLastError();
        closesocket(soc);
        freeaddrinfo(result);
        WSACleanup();
        return socket_state::BIND_FAIL;
    } else std::cout << "Socket bound to port\n";

    freeaddrinfo(result);

    return_code = listen(soc, ip.listen_count);
    if (return_code == SOCKET_ERROR) {
        std::cerr << "listen() failed with error: " << WSAGetLastError();
        closesocket(soc);
        freeaddrinfo(result);
        WSACleanup();
        return socket_state::LISTEN_FAIL;
    } else std::cout << "Listening . . .\n";

    return socket_state::PASS;
}

socket_state handle_requests(SOCKET &server, response_info &res) {
    int return_code;
    SOCKET client{INVALID_SOCKET};
    request_info req;
    do {
        client = accept(server, NULL, NULL);
        if (client == INVALID_SOCKET) {
            std::cerr << "Accept failed with error: " << WSAGetLastError();
            closesocket(server);
            WSACleanup();
            return socket_state::ACCEPT_FAIL;
        } else std::cout << "Client accepted\n";

        return_code = recv(client, req.buff, DEFAULT_BUFFLEN, false);
        if (return_code == 0) {
            std::cout << "Closing connection . . . \n";
            break;
        } else if (return_code == SOCKET_ERROR) {
            std::cerr << "recv() failed with error: " << WSAGetLastError();
            closesocket(client);
            WSACleanup();
            return socket_state::RECV_FAIL;
        } std::cout << "Recived: " << return_code << '\n';

        // std::cout << req.buff << '\n';

        http::parse(req.buff, req);
        http::send_response(client, req, res);

        closesocket(client);
        ZeroMemory(&req, sizeof(req));
    } while (return_code != 0);

    return_code = shutdown(client, SD_SEND);
    if (return_code == SOCKET_ERROR) {
        std::cout << "shutdown() failed with error: " << WSAGetLastError();
        return socket_state::SHUTDOWN_FAIL;
    } else std::cout << "Shutting down . . .";

    closesocket(server);
    WSACleanup();
    return socket_state::PASS;
}