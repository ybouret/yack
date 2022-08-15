
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
            struct eq_info
            {
                const char *name;
                const char *data;
                const char *kxpr;
                
                static const eq_info table[];
                static const size_t  count;
            };
        }
        
    }
    
}

#endif

