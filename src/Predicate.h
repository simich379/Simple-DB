#ifndef SIMPLEDBMS_PREDICATE_H
#define SIMPLEDBMS_PREDICATE_H

#include "Value.h"
#include "Type.h"

enum OpType
{
    EQ,  // =
    NE,  // <>
    LT,  // <
    LEQ, // <=
    GT,  // >
    GEQ  // >=
};

struct Predicate
{
    std::string &propertyName;
    OpType op;
    Value &val;
    Predicate(std::string &propertyName, OpType op, Value &val) : propertyName(propertyName), op(op), val(val) {}
};

#endif //SIMPLEDBMS_PREDICATE_H
