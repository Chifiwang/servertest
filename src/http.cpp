#include "http.hpp"
using namespace http;
// #define DEBUG
void set_request(const char* token, request &h);
void get_request_buff(const char *req, char *request_buff);
void mappify(std::string const& s, header_fields &h);

void http::parse(const char *req, request &h) {
    // process requestline
    char request_buff[MAX_BUFFLEN];
    get_request_buff(req, request_buff);

    char *request_line = strtok(request_buff, "\n");
    std::string header{strtok(NULL, "|")};
    [[maybe_unused]] char *body = strtok(NULL, "\0");

    char *req_type = strtok(request_line, " ");
    set_request(req_type, h);

    char *uri = strtok(NULL, " ");
    h.uri = uri;

    strtok(NULL, "/"); // throw away "HTML/" from html version
    char *maj = strtok(NULL, ".");
    h.maj_ver = std::atoi(maj);
    char *min = strtok(NULL, "\0");
    h.min_ver = std::atoi(min);

    // process headers
    mappify(header, h.headers);
    h.body = (body != NULL) ? body : "";
}

void mappify(std::string const& s, header_fields &h) {
    std::size_t ks = 0, ke, vs, ve;

    while ((ke = s.find(':', ks)) != std::string::npos) {
        if ((vs = s.find_first_not_of(": ", ke)) == std::string::npos) { break; }
        if ((ve = s.find('\n', vs)) == std::string::npos) { ve = s.size(); }

        h[s.substr(ks, ke-ks)] = s.substr(vs, ve-vs);
        ks = ve + 1;

        if(ve == s.size()) { break; }
    }
}

void get_request_buff(const char *req, char *request_buff) {
    strcpy(request_buff, req);
    // set up tokens
    for(u_int i = 0; i < strlen(request_buff) - 1; i++) {
        if(request_buff[i] == '\n' && request_buff[++i] == '\n') {
            request_buff[i+1] = '|';
            break;
        }
    }
}

void set_request(const char *token, request &h) { // std::map-ify
    if(strcmp(token, "GET") == 0) h.type = GET;
    else if(strcmp(token, "POST") == 0) h.type = POST; 
    else if(strcmp(token, "PUT") == 0) h.type = PUT;
    else if(strcmp(token, "HEAD") == 0) h.type = HEAD;
    else if(strcmp(token, "PATCH") == 0) h.type = PATCH;
    else if(strcmp(token, "DELETE") == 0) h.type = DELETE_;
    else if(strcmp(token, "CONNECT") == 0) h.type = CONNECT;
    else if(strcmp(token, "OPTIONS") == 0) h.type = OPTIONS;
    else if(strcmp(token, "TRACE") == 0) h.type = TRACE; 
}

void http::format(std::string &s, response &res) {
    {
        char buff[MAX_BUFFLEN];
        sprintf(
            buff, 
            (char *)HTTP_TEMPLATE, 
            res.maj_ver, 
            res.min_ver, 
            res.response_code, 
            res.response_type
        );
        s += buff;

    }

    typedef header_fields::iterator header_it;
    for (header_it i = res.headers.begin(); i != res.headers.end(); ++i) {
        s += i->first + ": " + i->second + '\n';
    } s += '\n';

    s += res.body;
}

int __stdcall get_send(const SOCKET *s, const char *buf, int len, int flags) {
    return send(*s, buf, len, flags);
}

state http::response::send() {
    std::string msg;
    format(msg, *this);

    int sent{};
    for (std::size_t tot = 0; tot < msg.size();) {
        sent = get_send(client, msg.data(), msg.size(), 0);
        if (sent < 0) {
            std::cerr << "Could not send response \n";
            return state::SEND_FAIL;
        }

        tot += sent;
    }
    return state::PASS;
}
