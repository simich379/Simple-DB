#include "ExecfileStatement.h"

namespace Interpreter{
    void ExecfileStatement::setFilePath(const std::string & s)
    {
        filePath = s;
    }

    std::string ExecfileStatement::getFilePath()
    {
        return filePath;
    }

    void ExecfileStatement::printStatement()
    {
        std::cout << "EXECFILE" << std::endl;
        std::cout << '\t' << filePath << std::endl;
    }

    void ExecfileStatement::callAPI()
    {
        throw std::logic_error("Execfile API does not exists.");
    }


}