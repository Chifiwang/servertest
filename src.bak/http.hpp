#ifndef HTTP_HPP
#define HTTP_HPP

#include "utils.hpp"

namespace http {

void parse(const char *request_buff, request_info &h);
void send_response(SOCKET &client, request_info &req, response_info &res);
void get_response(SOCKET &client, request_info &req, response_info &res);
// void format_res(request_info &req, response_info s&res, char *buff, int out_code, char *out);
}

#endif