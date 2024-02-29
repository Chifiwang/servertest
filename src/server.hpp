#ifndef SERVER_HPP
#define SERVER_HPP

#define NUM_THREADS 4

#include "utils.hpp"
#include "http.hpp"
#include "threads.hpp"

typedef void (*callable)(http::request, http::response);
typedef std::unordered_map<dir, callable> response_table;
typedef std::string_view ip_addr;

class base_server {
protected:
    const dir root;
    const ip_addr ip;
    const int port;
    response_table respond;
    SOCKET soc{INVALID_SOCKET};

    base_server() = default;
    base_server(dir r, ip_addr ip, int p) : root{r}, ip{ip}, port{p} {};
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
    const dir &get_root() { return root; }
};

class tcp_server : public base_server {
    // const dir root;
    // const ip_addr ip;
    // const int port;
    // response_table respond;
    // SOCKET soc{INVALID_SOCKET};
    friend int __stdcall get_listen(SOCKET &s, int l);

protected:
    void set_errors();
    state init();
    state read(SOCKET &c, char *buf);
    state send(SOCKET &c, http::request req, http::response res);
    state handle_requests();

public:
    tcp_server(dir root_dir, ip_addr ip, int port) : base_server(root_dir, ip, port) {}
    ~tcp_server() = default;

    void define(dir uri, callable f);
    state listen();
};

// template <class base_server>
class multithreading_mixin : public tcp_server{
protected:
    state handle_requests();
    state handle_client(char buf[MAX_BUFFLEN], SOCKET &c);
    friend void work(multithreading_mixin *p, char buf[MAX_BUFFLEN], SOCKET &c);
public:
    multithreading_mixin(dir root=DEFAULT_ROOT, ip_addr ip=DEFAULT_IP, int port=DEFAULT_PORT) : tcp_server(root, ip, port) {};
    // state listen();
};

template <class base_server>
class proxy_mixin {

public:
    proxy_mixin(dir root=DEFAULT_ROOT, ip_addr ip=DEFAULT_IP, int port=DEFAULT_PORT) : base_server(root, ip, port) {};
};

typedef tcp_server Server;
typedef multithreading_mixin tcp_async;
#endif