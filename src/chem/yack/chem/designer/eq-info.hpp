
//! \file

#ifndef YACK_CHEMICAL_DESIGNER_EQ_INFO_INCLUDED
#define YACK_CHEMICAL_DESIGNER_EQ_INFO_INCLUDED 1

#include "yack/string.hpp"

namespace yack
{
    namespace chemical
    {
        namespace nucleus
        {
            //__________________________________________________________________
            //
            //
            //! info for textual equilibrium
            //
            //__________________________________________________________________
            struct eq_info
            {
                const char *name; //!< name
                const char *data; //!< components
                const char *kxpr; //!< constant value
                
                static const eq_info table[]; //!< database
                static const size_t  count;   //!< number in database
            };
        }
        
    }
    
}

#endif

