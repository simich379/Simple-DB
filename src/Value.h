#ifndef SIMPLEDBMS_VALUE_H
#define SIMPLEDBMS_VALUE_H

#include "Type.h"


class Value
{
private:
    Type type;
    void *val;

public:
    Value() = default;
    Value( Type type, void *val) : type(type), val(val) {}
    Value( Type type, const void *val) : type(type)
    {
        setConst(val);
    }
    Value(const Value &that) : type(that.type) {

        val = that.val;
    }

    Value(const Value &that, int copyFlag) : type(that.type) {
        int size = that.type.getSize();
        val = new char[size];
        memcpy(val, that.val, size);
    }

    //convert integer types to pointer types and prev versa
    template <typename T>
    inline T *const getAsType() const
    {
        return reinterpret_cast<T *>(val);
    }

    inline void setConst(const void *val)
    {
        this->val = (void *)val;
    }



    inline void setType(Type &type)
    {
        this->type = type;
    }

    inline Type getType() const
    {
        return type;
    }

    bool operator!=(const Value &rhs) const { return !(*this == rhs); }
    bool operator>(const Value &rhs) const { return rhs < *this; }
    bool operator<=(const Value &rhs) const { return !(*this > rhs); }
    bool operator>=(const Value &rhs) const { return !(*this < rhs); }
    bool operator<(const Value &rhs) const
    {
        if (type.getBaseType() == BaseType::VARCHAR && rhs.getType().getBaseType() == BaseType::VARCHAR) {
            return std::strcmp((char *)val, (char *)rhs.val) < 0;
        }
        if (type.getBaseType() == BaseType::VARCHAR || rhs.getType().getBaseType() == BaseType::VARCHAR)
        {
            throw SQLException("cannot compare values with different types");
        }
        double thisValue = type.getBaseType() == BaseType::FLOAT ? *(double*)val : (double)(*(int*)val);
        double thatValue = rhs.getType().getBaseType() == BaseType::FLOAT ? *(double*)rhs.val : (double)(*(int*)rhs.val);
        return thisValue < thatValue;
    }

    bool operator==(const Value &rhs) const
    {
        if (type.getBaseType() == BaseType::VARCHAR && rhs.getType().getBaseType() == BaseType::VARCHAR) {
            return std::strcmp((char *)val, (char *)rhs.val) == 0;
        }
        if (type.getBaseType() == BaseType::VARCHAR || rhs.getType().getBaseType() == BaseType::VARCHAR)
        {
            throw SQLException("cannot compare values with different types");
        }
        double thisValue = type.getBaseType() == BaseType::FLOAT ? *(double*)val : (double)(*(int*)val);
        double thatValue = rhs.getType().getBaseType() == BaseType::FLOAT ? *(double*)rhs.val : (double)(*(int*)rhs.val);
        return thisValue == thatValue;
    }
};

#endif //SIMPLEDBMS_VALUE_H
