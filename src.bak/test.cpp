#include "utils.hpp"
#include "http.hpp"
#include "socket.hpp"
#include <bits/stdtr1c++.h>

int main() {

    // request_info h;
    // std::string s = "GET /software/htp/cics/index.html HTTP/1.1\ndummy: \n\n";
    // SOCKET soc{INVALID_SOCKET};
    // ip ipinfo;
    // ipinfo.ipaddr = "127.0.0.1";
    // ipinfo.port = 8080;
    // ipinfo.port_s = "8080";
    // ipinfo.listen_count = 2;

    // socket_state out = init_socket(soc, ipinfo);

    // http::parse(s.c_str(), h);
    // std::cout << h.type << ' ' << h.uri << ' ' << h.http_version;  
    // char dir[] = "/test.html";

    // std::string s = DEFAULT_ROOT;
    // s += dir;
    // printf(s.data());
    // printf("\n");
    // FILE *file = fopen(s.data(), "r");
    // if (file == NULL) std::cout << "fuck";
    // else {
    // char *t = (char *)"test";
    // // char buff[0xfff]{t};
    // char buff[512];
    // fread(buff, 1, sizeof(buff), file);
    // std::cout << buff;
    // }
    // char *out = (char *)malloc(DEFAULT_BUFFLEN*sizeof(char));
    // // request_info foo;
    // // foo.extention = "html";
    // // foo.maj_ver = 1;
    // // foo.min_ver = 1;
    // char buff[]{"<h>foo</h>"};
    // // response_info bar(1, "/");
    // // bar.body = (char *)"\n\n";
    // // http::format_res(foo, bar, buff, 200, out);

    // sprintf(out, (char *)HTTP_TEMPLATE, 1, 1, 231, "css");
    // memcpy(out + strlen(out), "", sizeof(""));
    // memcpy(out + strlen(out), "\n\n", sizeof("\n\n"));
    // memcpy(out + strlen(out), buff, sizeof(buff));
    // std::cout << out;

    
    return 0;
}