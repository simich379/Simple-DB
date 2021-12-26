#include "LexicalManager.h"
#include <stdexcept>
#include <ctype.h>


namespace Interpreter {
    LexicalManager::LexicalManager(std::istream & is): srcText(is)
    {
        lineNum = colNum = 0;
        pos = currLine.end();
        getNextLine();
    }

    std::vector<Token> LexicalManager::tokenize()
    {
        std::vector<Token> tokens;
        while (!reachEnd()) {
            tokens.push_back(getNextToken());
        }
        return tokens;
    }

    bool LexicalManager::reachEnd()
    {
        return pos == currLine.end() && !getNextLine();
    }

    void LexicalManager::skipSpaces(){
        while (pos != currLine.end() && (*pos == ' ' || *pos == '\t')) {
            skipChars(1);
        }
    }

    bool LexicalManager::getNextLine()
    {
        //reads characters from an input stream and places them into a string
        if (std::getline(srcText, currLine)) {
            lineNum++;
            colNum = 1;
            pos = currLine.begin();
            return true;
        }
        else return false;
    }

    void LexicalManager::skipChars(int n) {
        colNum += n;
        pos += n;
    }

    Token LexicalManager::getNextToken()
    {
        // read past and ignore any whitespace characters
        skipSpaces();
        if (pos == currLine.end()) {
            getNextLine();
            skipSpaces();
        }

        int currLineNum = lineNum;
        int currColNum = colNum;

        // Create a symbol token with line and column index
        if (*pos == '*') {
            skipChars(1);
            return Token(Symbol::ASTERISK, currLineNum, currColNum);
        }
        else if (*pos == ';') {
            skipChars(1);
            return Token(Symbol::SEMICOLON, currLineNum, currColNum);
        }
        else if (*pos == ',') {
            skipChars(1);
            return Token(Symbol::COMMA, currLineNum, currColNum);
        }
        else if (*pos == '(') {
            skipChars(1);
            return Token(Symbol::LBRACKET, currLineNum, currColNum);
        }
        else if (*pos == ')') {
            skipChars(1);
            return Token(Symbol::RBRACKET, currLineNum, currColNum);
        }
        else if (*pos == '<') {
            skipChars(1);
            if (*pos == '=') {
                skipChars(1);
                return Token(Symbol::LEQ, currLineNum, currColNum);
            }
            else if (*pos == '>') {
                skipChars(1);
                return Token(Symbol::NEQ, currLineNum, currColNum);
            }
            else return Token(Symbol::LT, currLineNum, currColNum);
        }
        else if (*pos == '>') {
            skipChars(1);
            if (*pos == '=') {
                skipChars(1);
                return Token(Symbol::GEQ, currLineNum, currColNum);
            }
            else return Token(Symbol::GT, currLineNum, currColNum);
        }
        else if (*pos == '=') {
            skipChars(1);
            return Token(Symbol::EQ, currLineNum, currColNum);
        }
//            // Create a string token with line and column index
        else if (*pos == '\'' || *pos == '"') {
            size_t foundEnd = currLine.substr(currColNum).find_first_of("\'\"");

            if (foundEnd != std::string::npos) {
                std::string s = currLine.substr(currColNum, foundEnd);
                skipChars(foundEnd + 2);
                return Token(TokenType::VARCHAR, s, currLineNum, currColNum);
            }
            else throw ParserError("Multi-line strings not supported.", currLineNum, currColNum);
        }
            // Create a number token with line and column index
        else if (isdigit(*pos)) {
            size_t foundEnd = currLine.substr(currColNum).find_first_not_of("0123456789.");
            if (currLine.substr(currColNum - 1, foundEnd + 1).find('.') != std::string::npos) {
                double f = atof(currLine.substr(currColNum - 1, foundEnd + 1).c_str());
                skipChars(foundEnd + 1);
                return Token(f, currLineNum, currColNum);
            }

            else {
                int i = atoi(currLine.substr(currColNum - 1, foundEnd + 1).c_str());
                skipChars(foundEnd + 1);
                return Token(i, currLineNum, currColNum);
            }
        }
            // Create a keyword or identifier token with line and column index
        else if (isalpha(*pos)) {
            //Searches the string for the first character that does not match any of the characters specified in its arguments.
            size_t foundEnd = currLine.substr(currColNum).find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_");
            std::string s = currLine.substr(currColNum - 1, foundEnd + 1);
            skipChars(foundEnd + 1);
            if (s == "AND") {
                return Token(Keyword::AND, currLineNum, currColNum);
            }

            else if (s == "CREATE") {
                return Token(Keyword::CREATE, currLineNum, currColNum);
            }
            else if (s == "DELETE") {
                return Token(Keyword::DELETE, currLineNum, currColNum);
            }
            else if (s == "DROP") {
                return Token(Keyword::DROP, currLineNum, currColNum);
            }
            else if (s == "EXECFILE") {
                return Token(Keyword::EXECFILE, currLineNum, currColNum);
            }
            else if (s == "FLOAT") {
                return Token(Keyword::FLOAT, currLineNum, currColNum);
            }
            else if (s == "FROM") {
                return Token(Keyword::FROM, currLineNum, currColNum);
            }

            else if (s == "INSERT") {
                return Token(Keyword::INSERT, currLineNum, currColNum);
            }
            else if (s == "INT") {
                return Token(Keyword::INT, currLineNum, currColNum);
            }
            else if (s == "BOOL") {
                return Token(Keyword::BOOL, currLineNum, currColNum);
            }
            else if (s == "INTO") {
                return Token(Keyword::INTO, currLineNum, currColNum);
            }
            else if (s == "KEY") {
                return Token(Keyword::KEY, currLineNum, currColNum);
            }
            else if (s == "ON") {
                return Token(Keyword::ON, currLineNum, currColNum);
            }
            else if (s == "PRIMARY") {
                return Token(Keyword::PRIMARY, currLineNum, currColNum);
            }
            else if (s == "QUIT") {
                return Token(Keyword::QUIT, currLineNum, currColNum);
            }
            else if (s == "SELECT") {
                return Token(Keyword::SELECT, currLineNum, currColNum);
            }
            else if (s == "TABLE") {
                return Token(Keyword::TABLE, currLineNum, currColNum);
            }
            else if (s == "UNIQUE") {
                return Token(Keyword::UNIQUE, currLineNum, currColNum);
            }
            else if (s == "VALUES") {
                return Token(Keyword::VALUES, currLineNum, currColNum);
            }
            else if (s == "WHERE") {
                return Token(Keyword::WHERE, currLineNum, currColNum);
            }
            else {
                return Token(TokenType::IDENTIFIER, s, currLineNum, currColNum);
            }
        }
        else {
            throw ParserError("Unknown Token.", currLineNum, currColNum);
        }
    }
}
