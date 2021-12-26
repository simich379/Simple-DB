#ifndef SIMPLEDBMS_PROPERTY_H
#define SIMPLEDBMS_PROPERTY_H

#include "Value.h"
#include <string>

struct Property
{
    Type type;
    char name[64];
    bool unique;

    Property() = default;
    Property( const Type type, bool unique, std::string &name) : type(type), unique(unique)
    {
        strcpy(this->name, name.c_str());
    }
};
#endif //SIMPLEDBMS_PROPERTY_H
