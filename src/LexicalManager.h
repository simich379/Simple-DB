/*
	- SPACE, TAB, NEWLINE count as whitespace.
*/

#pragma once
#include"Token.h"
#include <string>
#include <vector>
#include <iostream>
#include "ParserError.h"

namespace Interpreter {

    class LexicalManager
    {
    public:
        LexicalManager(std::istream &is);
        std::vector<Token> tokenize();

    private:
        bool reachEnd();
        bool getNextLine();
        void skipChars(int n);
        void skipSpaces();
        Token getNextToken();

        std::istream &srcText;
        std::string currLine;
        std::string::iterator pos;
        int lineNum, colNum;
    };
}