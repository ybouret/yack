#include "yack/chem/designer/cm-info.hpp"

namespace yack
{
    namespace chemical
    {

        namespace nucleus
        {
            cm_info:: ~cm_info() throw() 
            {
            }

            cm_info:: cm_info() : sp_info(), nu(1)
            {
            }

            cm_info:: cm_info(const cm_info &other) :
            sp_info(other),
            nu(other.nu)
            {
            }

            
        }

    }

}

