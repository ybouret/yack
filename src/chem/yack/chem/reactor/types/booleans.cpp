
#include "yack/chem/reactor/types/booleans.hpp"

namespace yack {

    namespace chemical {


        booleans::  booleans(const size_t n, const bool f) : object(), cxx_array<bool>(n,f) {}
        booleans:: ~booleans() noexcept {}


    }

}


