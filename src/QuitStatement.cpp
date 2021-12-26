#include "QuitStatement.h"

namespace Interpreter{
    void QuitStatement::callAPI()
    {
        throw std::logic_error("Quit API does not exists.");
    }

    void QuitStatement::printStatement()
    {
        std::cout << "QUIT" << std::endl;
    }
}