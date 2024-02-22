//Compile command: g++ main.cpp server.cpp http.cpp query.cpp -o main -lws2_32
#include "server.hpp"

void tcp_server::get(dir uri, callable f) { respond[uri] = f; }

state tcp_server::listen() {
    get(ERROR_404, [](http::request req, http::response res) -> void {
        res.headers["Content-Type"] = "text/html";

        res.body = 
            "<html>" \
            "<head><title>404 Not Found</title></head>" \
            "<body bgcolor=\"white\">" \
            "<center><h1>404 Not Found</h1></center>" \
            "<hr><center>server-test</center>" \
            "</body>" \
            "</html>";

        res.headers["Content-Length"] = std::to_string(res.body.size());
        res.send();
    });

    state soc_state;

    soc_state = init();
    if (soc_state != state::PASS) { return soc_state; }

    soc_state = handle_requests();
    return soc_state;
}

/**** HELPERS ****/
// external helper
int __stdcall get_listen(SOCKET &s, int l) { return listen(s, l); }

state tcp_server::init() {
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
    localaddr.sin_addr.s_addr = inet_addr(ip.data());
    localaddr.sin_port = htons(port);

    localaddr_size = sizeof(localaddr);

    return_code = WSAStartup(MAKEWORD(1, 1), &wsa_data);
    if (return_code) {
        std::cerr << "WSAStartup() failed with code: " << return_code;
        return socket_state::STARTUP_FAIL;
    } else std::cout << "Startup successful\n";

    return_code = getaddrinfo(NULL, std::to_string(port).data(), &hints, &result);
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

    return_code = get_listen(soc, DEFAULT_LISTENER_COUNT);
    if (return_code == SOCKET_ERROR) {
        std::cerr << "listen() failed with error: " << WSAGetLastError();
        closesocket(soc);
        freeaddrinfo(result);
        WSACleanup();
        return socket_state::LISTEN_FAIL;
    } else std::cout << "Listening . . .\n";

    return socket_state::PASS;
}

state tcp_server::read(SOCKET &c, char *buf) {
        int return_code;
        c = accept(soc, NULL, NULL);
        if (c == INVALID_SOCKET) {
            std::cerr << "Accept failed with error: " << WSAGetLastError();
            closesocket(soc);
            WSACleanup();
            return socket_state::ACCEPT_FAIL;
        } else std::cout << "Client accepted\n";

        return_code = recv(c, buf, DEFAULT_BUFFLEN, false);
        if (return_code == 0) {
            std::cout << "Closing connection . . . \n";
            return socket_state::CLOSE;
        } else if (return_code == SOCKET_ERROR) {
            std::cerr << "recv() failed with error: " << WSAGetLastError();
            closesocket(c);
            closesocket(soc);
            WSACleanup();
            return socket_state::RECV_FAIL;
        } std::cout << "Recived " << return_code << " bytes\n";

        return socket_state::PASS;
}

state tcp_server::handle_requests() {
    state err;
    SOCKET client{INVALID_SOCKET};

    do {
        char buf[MAX_BUFFLEN];

        err = read(client, buf);

        if (err == state::CLOSE) { break; }
        else if (err != state::PASS) { return err; }

        http::request req;
        http::parse(buf, req);
        
        http::response res{client, root + req.uri};
        res.maj_ver = 1;
        res.min_ver = 1;
        res.response_code = 200;
        res.response_type = OK;

        if (respond.find(req.uri) == respond.end()) { req.uri = ERROR_404; }
        respond[req.uri](req, res);

        closesocket(client);
    } while (err == state::PASS);

    int return_code;
    return_code = shutdown(client, SD_SEND);
    if (return_code == SOCKET_ERROR) {
        std::cout << "shutdown() failed with error: " << WSAGetLastError();
        return socket_state::SHUTDOWN_FAIL;
    } else std::cout << "Shutting down . . .";

    closesocket(soc);
    WSACleanup();
    return socket_state::PASS;
}