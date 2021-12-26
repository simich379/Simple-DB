#pragma once
#include "ParserError.h"
#include "LexicalManager.h"
#include "Token.h"
#include "Statement.h"
#include "CreateTableStatement.h"
#include "CreateIndexStatement.h"
#include "DropIndexStatement.h"
#include "DropStatement.h"
#include "DeleteStatement.h"
#include "InsertStatement.h"
#include "SelectStatement.h"
#include "QuitStatement.h"
#include "ExecfileStatement.h"
#include <vector>

namespace Interpreter {
    typedef std::shared_ptr<Statement> PtrStat;
    typedef std::vector<PtrStat> Statements;

    class Parser {
    private:
        LexicalManager lexer;
        std::vector<Token> tokens;
        std::vector<Token>::iterator itr;

    public:
        Parser(std::istream & is);
        Statements parse();
        // utilities
        void raiseException(const std::string & msg);
        void expect(const Keyword &);	// parse a specific keyword, raise exception if keyword not found
        void expect(const Symbol &);	// parse a specific symbol, raise exception if symbol not found
        bool meet(const Keyword &)const;	// check for a specific keyword (do not proceed)
        bool meet(const Symbol &)const; 		// check for a specific symbol (do not proceed)
        // functions to get a specific type of token and proceed
        std::string getIdentifier()  ;
        Type getAttributeType();
        Predicate getPredicate();
        Value getValue();
        int getInteger();
        double getFloat();
        bool getBool();
        std::string getString();
        // parse a specific type of statement
        PtrStat parseCreate();
        PtrStat parseDrop();
        PtrStat parseCreateTable();
        PtrStat parseCreateIndex();
        PtrStat parseDropTable();
        PtrStat parseDropIndex();
        PtrStat parseSelect();
        PtrStat parseInsert();
        PtrStat parseDelete();
        PtrStat parseQuit();
        PtrStat parseExecFile();
    };
}