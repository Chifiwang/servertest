#include "server.hpp"

int main(int argc, char *argv[]) {
    state err;

    Server s(DEFAULT_ROOT, DEFAULT_IP, DEFAULT_PORT);

    s.get("/test.html", [](http::request req, http::response res) -> void {
        res.headers["Content-Type"] = "text/html";
        res.body = query::load(res.path);
        res.send();
    });

    s.get("/style.css", [](http::request req, http::response res) -> void {
        res.headers["Content-Type"] = "text/css";
        res.body = query::load(res.path);

        res.send();
    });

    err = s.listen();
    if (err != state::PASS) {
        // TODO
    } 

   return 0;
}