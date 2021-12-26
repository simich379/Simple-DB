#ifndef SIMPLEDBMS_EXCEPTION_H
#define SIMPLEDBMS_EXCEPTION_H

#include <string>

struct SQLException {
    const std::string msg;
    SQLException(const std::string msg) : msg(msg) {}
};

#endif //SIMPLEDBMS_EXCEPTION_H
