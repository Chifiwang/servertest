#ifndef QUERY_HPP
#define QUERY_HPP

#include "utils.hpp"

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

namespace query {

std::string load(dir path);
bool exists(dir path, int mode=F_OK);

};
#endif