#ifndef SIMPLEDBMS_CHARWRAPPER_H
#define SIMPLEDBMS_CHARWRAPPER_H
#include <string>
#include "Value.h"

int inline strcmp(const char *str1, const char *str2)
{
    while (*str1 == *str2) {
        if (*str1 == '\0') return 0;
        str1++;
        str2++;
    }
    return *str1 - *str2;
}

class Value;

struct CharWrapper
{


    char content[255];
    CharWrapper() = default;

    CharWrapper(const Value* value) {
        memcpy(content, value->getAsType<char>(), value->getType().getSize() * sizeof(char));
    }
    CharWrapper& operator =(CharWrapper& obj) {
        memcpy(content, obj.content, sizeof(content));
        return *this;
    }

    bool operator <(const CharWrapper& another) const { return strcmp(content, another.content) < 0; }
    bool operator >(const CharWrapper& another)  const { return strcmp(content, another.content) > 0; }
    bool operator <=(const CharWrapper& another)const  { return strcmp(content, another.content) <= 0; }
    bool operator >=(const CharWrapper& another) const  { return strcmp(content, another.content) >= 0; }
    bool operator ==(const CharWrapper& another) const { return strcmp(content, another.content) == 0; }
    bool operator !=(const CharWrapper& another) const { return strcmp(content, another.content) != 0; }
    friend std::ostream & operator<<(std::ostream & os, const CharWrapper & c) {
        std::string s(c.content);
        os << s;
        return os;
    }
};

#endif //SIMPLEDBMS_CHARWRAPPER_H
