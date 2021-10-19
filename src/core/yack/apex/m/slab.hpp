
//! \file

#ifndef YACK_APEX_SLAB_INCLUDED
#define YACL_APEX_SLAB_INCLUDED 1

#include "yack/arith/ilog2.hpp"

namespace yack
{
    
    namespace apex
    {
        class slab
        {
        public:
            typedef uint16_t item_t;
            item_t                    first_available; //!< bookkeeping
            item_t                    still_available; //!< bookkeeping
            item_t                    operated_number; //!< bookkeeping
            const item_t              provided_number; //!< initial count : still_available + operated_number
            item_t                   *data;            //!< first item
            const item_t * const      last;            //!< first invalid item
            slab                      *next;            //!< for list
            slab                      *prev;            //!< for list
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(slab);
        };
    }
    
}

#endif

