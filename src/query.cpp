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

// query::node *query::make_dir(dir root, int foo, paths *exclude) {
//     namespace fs = std::filesystem;
//     for (const fs::__cxx11::directory_entry &f : fs::directory_iterator(root)) {
//         std::cerr << f.path().string() << "\n";
//     }

//     return new query::node;
// }