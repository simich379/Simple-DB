
#include "DropIndexStatement.h"

namespace Interpreter{
    void DropIndexStatement::setIndexName(const std::string & s)
    {
        indexName = s;
    }

    void DropIndexStatement::callAPI()
    {
        API::SQLResult<void> res = API::dropIndex(indexName);

        if (res.isSuccess) {
            std::cout << "Index \'" << indexName
                      << "\' dropped successfully in " << res.durationMS
                      << " ms" << std::endl;
        }
        else {
            std::cout << "Drop Index fails: " + res.errorMessage << std::endl;
        }
    }

    void DropIndexStatement::printStatement()
    {
        std::cout << "DROP INDEX" << std::endl;
        std::cout << "\t " << indexName << std::endl;
    }
}