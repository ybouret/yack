
#include "yack/chem/designer/sp-info.hpp"

namespace yack
{
    namespace chemical
    {

        namespace nucleus
        {
            sp_info::  sp_info() : name(), z(0) {}
            sp_info::  sp_info(const sp_info &si) : name(si.name), z(si.z) {}
            sp_info:: ~sp_info() noexcept {}
        }

    }

}

