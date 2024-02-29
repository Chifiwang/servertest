#include "server.hpp"

int main(int argc, char *argv[]) {
    state err;

    Server s(DEFAULT_ROOT, DEFAULT_IP, DEFAULT_PORT);

    s.define("/test.html", [](http::request req, http::response res) -> void {
        res.headers["Content-Type"] = "text/html";
        res.body = query::load(res.path);
        res.send();
    });

    s.define("/style.css", [](http::request req, http::response res) -> void {
        res.headers["Content-Type"] = "text/css";
        res.body = query::load(res.path);
        res.send();
    });

    s.define("/favicon.png", [](http::request req, http::response res) -> void {
        // std::cout << "Favicon response sent\n";
        res.headers["Content-Type"] = "image/png";
        res.body = query::load_png(res.path); // not working
        res.send();
    });

    err = s.listen();
    if (err != state::PASS) {
        // TODO
    }
   return 0;
}