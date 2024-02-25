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

bool query::exists(dir path, int mode) {
    return access(path.data(), mode) == 0;
}

std::string query::load_png(dir path) {
    std::ifstream f;
    f.open(path, std::ios::in | std::ios::binary);
    
    f.seekg(0, std::ios::end);
    size_t s = f.tellg();

    char buf[s];
    f.read(buf, s);
    buf[s] = '\0';
    std::string out{buf};
    return out;
}