#ifndef HTTP_HPP
#define HTTP_HPP

#include "utils.hpp"
#include "query.hpp"

#define OK "OK"
#define NOT_FOUND "Not Found"
#define ERROR_404 "404"

namespace http {
typedef std::unordered_map<std::string, std::string> header_fields;
typedef std::string body_text;

struct request {
    request_type type;
    dir uri;
    int maj_ver;
    int min_ver;
    header_fields headers;
    body_text body;
};

struct response {

    int response_code;
    int maj_ver;
    int min_ver;
    std::string response_type;
    header_fields headers;
    body_text body;

    const dir path;

    response(SOCKET &c, dir r) : client{&c}, path{r} {};
    state send();
private:
    const SOCKET *client;
};

void parse(const char *request_buff, request &req);
void format(std::string &s, response &res);
void throw_404();
}

#endif