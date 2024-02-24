#ifndef QUERY_HPP
#define QUERY_HPP

#include "utils.hpp"
#include <filesystem>
#include <unordered_set>
#include <deque>
#ifndef WIN32
    #include <unistd.h>
#else
    #include <io.h>
    // #define F_OK
    // #define R_OK
    // #define W_OK
    // #define X_OK
    #define access _access
#endif

typedef std::unordered_set<dir> paths;
typedef std::unordered_set<std::string> files;

namespace query {

std::string load(dir path);
bool exists(dir path, int mode=F_OK);
// node *make_dir(dir root, int foo, paths *exclude=NULL);

};
#endif