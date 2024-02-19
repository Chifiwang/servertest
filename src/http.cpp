#include "http.hpp"
using namespace http;

// #define DEBUG

void set_request(const char* token, request_info &h);
void get_request_buff(const char *req, char *request_buff);
long int get_file_len(std::string &dir);
void mappify(std::string const& s, header_fields &h);

void http::parse(const char *req, request_info &h) {
    // process requestline
    char request_buff[MAX_BUFFLEN];
    get_request_buff(req, request_buff);

    char *request_line = strtok(request_buff, "\n");
    std::string header{strtok(NULL, "|")};
    [[maybe_unused]] char *body = strtok(NULL, "\0");

    char *req_type = strtok(request_line, " ");
    set_request(req_type, h);

    char *uri = strtok(NULL, ".");
    h.uri = uri;

    char *ext = strtok(NULL, " ");
    h.extention = ext;

    strtok(NULL, "/"); // throw away "HTML/" from html version
    char *maj = strtok(NULL, ".");
    h.maj_ver = std::atoi(maj);
    char *min = strtok(NULL, "\0");
    h.min_ver = std::atoi(min);

    // process headers
    mappify(header, h.headers);
    
}

void http::send_response(SOCKET &client, request_info &req, response_info &res) {
    switch (req.type) {
        case GET:
            get_response(client, req, res);
            break;
        case POST:
            break;
        case PUT:
            break;
        case HEAD:
            break;
        case PATCH:
            break;
        case DELETE_:
            break;
        case CONNECT:
            break;
        case OPTIONS:
            break;
        case TRACE:
            break;
        default:
            std::cerr << "Invalid HTTP request.\n"; 
    }
    return;
}

void http::get_response(SOCKET &client, request_info &req, response_info &res) {
    std::string dir = res.root_dir + req.uri + "." + req.extention;
    long int size{get_file_len(dir)};
    if (size < 0) {
        std::cerr << "File not found \n";
        return;
    }
    
    FILE *f = fopen(dir.data(), "r");
    char buff[MAX_BUFFLEN];
    ZeroMemory(buff, sizeof(buff));

    // make a formatter and also make 206 responses possible
    std::string msg = "HTTP/1.1 200 OK\nContent-Type: text/" + req.extention + 
                        "\nAccess-Control-Allow-Credentials:true\n\n";

    while (fread(buff, 1, sizeof(buff), f)) {
        msg += buff;
    } // std::cout << msg << '\n';

    {
        int sent{};
        for(std::size_t tot = 0; tot < msg.size();) {
            sent = send(client, msg.data(), msg.size(), 0);
            if (sent < 0) {
                std::cerr << "Could not send response \n";
            }

            tot += sent;
        }
    }
    fclose(f);
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

void set_request(const char *token, request_info &h) { // std::map-ify
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

long int get_file_len(std::string &dir) {
    FILE *f = fopen(dir.data(), "r");
    if (f == NULL) {
        std::cerr << "503 Invalid directory\n";
        return -1;
        // ...
    }
    if (fseek(f, 0L, SEEK_END) < 0) return -1;

    long int tot = ftell(f);
    fclose(f);

    return tot;
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

#ifdef DEBUG
int main() {
    header_fields h{};
    mappify("A: a\nB: b\nC: c\n", h);
    for (auto &s: h) {
        std::cout << s.first << " " << s.second << '\n';
    }
    return 0;
}
#endif