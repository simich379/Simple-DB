#pragma once
#include "Parser.h"
#include "ParserError.h"
//Reading evaluate and printing
namespace Interpreter {
    class REPL {
    public:
        REPL() = default;

        ~REPL() = default;

        void run();

    private:
        std::string line;
        std::stringstream buffer;
        bool isRunning;
        bool isReading;
    };
}