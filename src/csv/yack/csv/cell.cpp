
#include "yack/csv/cell.hpp"

namespace yack
{

    namespace CSV
    {
        Cell:: Cell(const Cell &other) : collection(), string(other), type(other.type), next(0), prev(0) {}

        Cell:: ~Cell() noexcept
        {
        }

        Cell:: Cell() : collection(), string(), type(isRaw), next(0), prev(0) {}
        
    }

}

