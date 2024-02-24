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

    s.define("/favicon.ico", [](http::request req, http::response res) -> void {
        res.headers["Content-Type"] = "image/x-icon";
        res.body = query::load(res.path);
        res.send();
    });

    err = s.listen();
    if (err != state::PASS) {
        // TODO
    } 
    // query::make_dir(DEFAULT_ROOT, 0);

   return 0;
}