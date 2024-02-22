#include "query.hpp"

std::string query::load(dir path) {
    std::string contents;
    char buf[MAX_BUFFLEN];
    ZeroMemory(buf, sizeof(buf));

    FILE *f = fopen(path.data(), "r");
    
    while (fread(buf, 1, sizeof(buf), f)) {
        contents += buf;
    }

    return contents;
}