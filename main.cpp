#include <iostream>
#include <fstream>
#include <algorithm>
#include "src/LexicalManager.h"
#include "src/Token.h"
#include "src/Type.h"
#include "src/Table.h"
#include "src/Property.h"
#include "src/Statement.h"
#include "src/Parser.h"
#include "src/REPL.h"
#include "src/Parser.h"
#include "src/REPL.h"
#include "src/LexicalManager.h"
#include "src/Token.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <algorithm>
//#include <crtdbg.h>

#define IFILENAME "lexer_test.sql"

using namespace Interpreter;


//void testLexer() {
//    std::ifstream fin(IFILENAME);
//    if (!fin) {
//        std::cout << "FILE ERROR" << std::endl;
//    }
//    else {
//        std::istream & is = fin;
//        LexicalManager lexer(is);
//
//        std::vector<Token> tokens = lexer.tokenize();
//
//        std::for_each(tokens.begin(), tokens.end(), printToken);
//    }
//}

void testParser(){


    std::ofstream out(IFILENAME, std::ios::trunc);

    std::ifstream fin(IFILENAME);
    if (!fin) {
        std::cout << "FILE ERROR" << std::endl;
    }
    else {
        std::istream & is = fin;
        Parser parser(is);

        Statements statements = parser.parse();

        for (PtrStat statement : statements) {
            statement->printStatement();
        }
    }
}

void testREPL() {
    REPL repl;
    repl.run();
}

int main() {
    //_CrtSetBreakAlloc(745);

    testParser();
    testREPL();
//    _CrtDumpMemoryLeaks();
    return 0;
}


