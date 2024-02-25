//Compile command: g++ main.cpp server.cpp http.cpp query.cpp -o main -lws2_32
#include "server.hpp"

void tcp_server::define(dir uri, callable f) { respond[uri] = f; }

state tcp_server::listen() {
    set_errors();

    state soc_state;
    soc_state = init();
    if (soc_state != state::PASS) { return soc_state; }

    soc_state = handle_requests();
    return soc_state;
}

void tcp_server::set_errors() {
    define(ERROR_404, [](http::request req, http::response res) -> void {
        res.headers["Content-Type"] = "text/html";
        res.response_code = 404;
        res.response_type = NOT_FOUND;

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
        std::cerr << ERROR_404 << '\n';
    });

    define(ERROR_505, [](http::request req, http::response res) -> void {
        res.headers["Content-Type"] = "text/html";
        res.response_code = 505;
        res.response_type = NOT_SUPPORTED;

        res.body = 
            "<html>" \
            "<head><title>505 HTTP Version Not Supported</title></head>" \
            "<body bgcolor=\"white\">" \
            "<center><h1>505 HTTP Version Not Supported</h1></center>" \
            "<hr><center>server-test</center>" \
            "</body>" \
            "</html>";

        res.headers["Content-Length"] = std::to_string(res.body.size());
        res.send();
    });

    define(ERROR_418, [](http::request req, http::response res) -> void {
        res.headers["Content-Type"] = "text/html";
        res.response_code = 418;
        res.response_type = TEAPOT;

        res.body = 
            "<html>" \
            "<head><title>418 I'm a Teapot</title></head>" \
            "<body bgcolor=\"white\">" \
            "<center><h1>418 I'm a Teapot</h1></center>" \
            "<hr><center>This server is a teapot, and it cannot brew coffee.</center>" \
            "</body>" \
            "</html>";

        res.headers["Content-Length"] = std::to_string(res.body.size());
        res.send();
    });   
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

state tcp_server::send(SOCKET &c, http::request req, http::response res) {
    if (respond.find(req.uri.path) == respond.end()) {
        respond[ERROR_404](req, res);
        return state::SEND_FAIL;
    } else if (req.maj_ver != 1) {
        respond[ERROR_505](req, res);
        return state::SEND_FAIL;
    }

    respond[req.uri.path](req, res);
    return state::PASS;
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
        
        http::response res{client, root + req.uri.path};
        res.maj_ver = 1;
        res.min_ver = 1;
        res.response_code = 200;
        res.response_type = OK;
        send(client, req, res);

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