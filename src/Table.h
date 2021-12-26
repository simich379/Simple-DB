#ifndef SIMPLEDBMS_TABLE_H
#define SIMPLEDBMS_TABLE_H

#include "Type.h"
#include "Property.h"
struct Table
{
    std::string &tableName;
    std::string &primaryKey;
    std::vector<Property> &properties;
    Table(std::string &tableName, std::string &primaryKey, std::vector<Property> &properties)
            : tableName(tableName), primaryKey(primaryKey), properties(properties) {}
};
#endif //SIMPLEDBMS_TABLE_H
