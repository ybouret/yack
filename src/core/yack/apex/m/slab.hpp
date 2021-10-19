
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
               
            uint8_t                    first_available; //!< bookkeeping in [0..127]
            uint8_t                    still_available; //!< bookkeeping in [0..128]
            uint8_t                    operated_number; //!< bookkeeping in [0..128]
            const uint8_t              provided_number; //!< initial count : still_available + operated_number in [0..128]
          
            const size_t               block_size;
            uint8_t                   *data;            //!< first item
            const uint8_t * const      last;            //!< first invalid item
            slab                      *next;            //!< for list
            slab                      *prev;            //!< for list
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(slab);
        };
    }
    
}

#endif

