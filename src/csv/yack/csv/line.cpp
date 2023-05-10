#include "yack/csv/line.hpp"

namespace yack
{

    namespace CSV
    {

        Line::  Line() noexcept : Cells(), next(0), prev(0) {}
        Line:: ~Line() noexcept {}
        Line:: Line(const Line &other) : Cells(other), next(0), prev(0) {}

    }

}


