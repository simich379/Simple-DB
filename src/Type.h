#ifndef SIMPLEDBMS_TYPE_H
#define SIMPLEDBMS_TYPE_H

#include <string>
#include <vector>
#include <memory>
#include <cstring>
#include "Exception.h"

enum BaseType
{
    INT,
    FLOAT,
    VARCHAR,
    BOOL
};

class Type
{
private:
    BaseType baseType;
    size_t size;

public:
    Type() = default;

    Type(const BaseType baseType, size_t charSize = 0)
    {
        setType(baseType, charSize);
    }

    Type(const Type &that) : baseType(that.baseType), size(that.size) {}

    inline void setType( const BaseType type, size_t charSize = 0)
    {
        this->baseType = type;
        switch (type)
        {

            case BaseType::BOOL:
                size = sizeof(bool);
                break;

            case BaseType::INT:
                size = sizeof(int);
                break;
            case BaseType::FLOAT:
                size = sizeof(double);
                break;
            case BaseType::VARCHAR:
                size = charSize;
                break;
        }
    }

    inline size_t getSize() const
    {
        return size;
    }
    inline BaseType getBaseType() const
    {
        return baseType;
    }

    bool operator==(const Type &rhs) const
    {
        return baseType == rhs.baseType && size == rhs.size;
    }

    bool operator!=(const Type &rhs) const
    {
        return !(rhs == *this);
    }
};



#endif //SIMPLEDBMS_TYPE_H
