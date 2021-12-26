#ifndef SIMPLEDBMS_TOKEN_H
#define SIMPLEDBMS_TOKEN_H

#include <string>
#include <stdlib.h>

namespace Interpreter {

    enum class Symbol {
        // one-character symbols
        ASTERISK,
        SEMICOLON,
        COMMA,
        LBRACKET,
        RBRACKET,
        LT,
        GT,
        EQ,
        // two-character symbols
        LEQ,
        GEQ,
        NEQ
    };

    enum class Keyword {
        AND,
        BOOL,
        CREATE,
        DELETE,
        DROP,
        EXECFILE,
        FLOAT,
        FROM,
        INSERT,
        INT,
        VARCHAR,
        INTO,
        KEY,
        ON,
        PRIMARY,
        QUIT,
        SELECT,
        TABLE,
        UNIQUE,
        VALUES,
        WHERE, INDEX
    };

    enum class TokenType
    {
        SYMBOL,
        KEYWORD,
        IDENTIFIER,
        INTEGER,
        FLOAT,
        VARCHAR,
        BOOL
    };

    struct TokenValue {
        Symbol svalue;
        Keyword kvalue;
        std::string strvalue;	// IDENTIFIER or VARCHAR
        int intvalue;
        float fvalue;
        bool bvalue;
    };


    class Token
    {
    public:
        Token(const Symbol & sv,  int ln,  int cn);
        Token(const Keyword & kv,  int ln,  int cn);
        Token(TokenType t,  std::string & strv,  int ln,  int cn);
        Token( int intv, int ln,  int cn);
        Token(double fv,  int ln,  int cn);
        Token(bool bv, int ln,  int cn);

        friend void printToken(const Token & token);
        TokenType getType();
        TokenValue getValue();
        int getlineNum() const;
        int getcolNum() const;

    private:
        TokenType type;
        TokenValue value;
        int lineNum, colNum;
    };


    const char *mapKeywordToString(const Keyword &);
    const char *mapSymbolToString(const Symbol &);
    const char *mapTokenTypeToString(const TokenType &);

}

#endif //SIMPLEDBMS_TOKEN_H
