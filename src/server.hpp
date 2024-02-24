#ifndef SERVER_HPP
#define SERVER_HPP

#include "utils.hpp"
#include "http.hpp"

typedef void (*callable)(http::request, http::response);
typedef std::unordered_map<dir, callable> response_table;
typedef std::string_view ip_addr;

class base_server {
protected:
    base_server() = default;
    virtual ~base_server() = default;

    virtual void set_errors() = 0;
    virtual state init() = 0;
    virtual state read(SOCKET &c, char *buf) = 0;
    virtual state send(SOCKET &c, http::request req, http::response res) = 0;
    virtual state handle_requests() = 0;

public:
    friend state http::response::send();
    /* TODO add more http response types*/
    virtual void define(dir uri, callable f) = 0;
    virtual state listen() = 0;

};

class tcp_server : public base_server {
    const dir root;
    const ip_addr ip;
    const int port;
    response_table respond;
    SOCKET soc{INVALID_SOCKET};
    friend int __stdcall get_listen(SOCKET &s, int l);

protected:
    void set_errors();
    state init();
    state read(SOCKET &c, char *buf);
    state send(SOCKET &c, http::request req, http::response res);
    state handle_requests();

public:
    tcp_server(dir root_dir, ip_addr ip, int port) : root{root_dir}, ip{ip}, port{port} {}
    ~tcp_server() = default;

    void define(dir uri, callable f);
    state listen();
};

typedef tcp_server Server;
#endif