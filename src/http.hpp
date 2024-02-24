#ifndef HTTP_HPP
#define HTTP_HPP

#include "utils.hpp"
#include "query.hpp"

#define OK "OK"
#define ERROR_404 "404"
#define NOT_FOUND "Not Found"

#define ERROR_505 "505"
#define NOT_SUPPORTED "HTTP Version Not Supported"

#define ERROR_418 "418"
#define TEAPOT "I'm a teapot"

namespace http {
typedef std::unordered_map<std::string, std::string> fields;
typedef std::string body_text;

struct uri {
    dir path;
    fields queries;
    fields fragments;
};

struct request {
    request_type type;
    uri uri;
    int maj_ver;
    int min_ver;
    fields headers;
    body_text body;
};

struct response {
    int response_code;
    int maj_ver;
    int min_ver;
    std::string response_type;
    fields headers;
    body_text body;

    const dir path;
public:
    response(SOCKET &c, dir r) : path{r}, client{&c} {};
    state send();
private:
    const SOCKET *client;
};

void parse(const char *request_buff, request &req);
void format(std::string &s, response &res);
}

#endif