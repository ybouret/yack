#include "yack/system/hw.hpp"

extern "C" size_t upsylon_hardware_nprocs();

namespace yack
{

    size_t hardware:: nprocs()
    {
        return upsylon_hardware_nprocs();
    }

}
