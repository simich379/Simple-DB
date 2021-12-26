#include "Statement.h"
#include "API.h"
#include "Exception.h"
#include "Property.h"
#include "Predicate.h"
#include "Table.h"
#include <sstream>

namespace Interpreter {
    class CreateTableStatement : public Statement {
    public:
        void callAPI() override;
        void printStatement() override;
        std::basic_string<char> setTableName(const std::string &);
        void setPrimaryKey(std::string &);
        void addProperty(const Property &);

    private:
        std::string tableName;
        std::string primaryKey;
        std::vector<Property> properties;
        std::vector<Predicate> predicates;
    };
}