#ifndef QUERY_HPP
#define QUERY_HPP

#include "utils.hpp"
// #define DEV
#ifdef DEV
#include <filesystem>
#include <unordered_set>
#include <deque>

typedef std::unordered_set<dir> paths;
typedef std::unordered_set<std::string> files;
#endif

#ifndef WIN32
    #include <unistd.h>
#else
    #include <io.h>
    #include <fstream>
    #define access _access
#endif

namespace query {

std::string load(dir path);
bool exists(dir path, int mode=F_OK);
std::string load_png(dir path);
};
#endif