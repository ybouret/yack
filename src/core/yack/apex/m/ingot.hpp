//! \file

#ifndef YACK_APEX_INGOT_INCLUDED
#define YACK_APEX_INGOT_INCLUDED 1

#include "yack/memory/ownership.hpp"

namespace yack
{
    
    namespace apex
    {
        class ingot
        {
        public:
            static const size_t max_blocks = 128;
            
            uint8_t                    first_available; //!< bookkeeping in [0..127]
            uint8_t                    still_available; //!< bookkeeping in [0..128]
            uint8_t                    operated_number; //!< bookkeeping in [0..128]
            const uint8_t              provided_number; //!< initial count : still_available + operated_number in [0..128]
            
            uint8_t                   *data;            //!< first item
            const uint8_t * const      last;            //!< first invalid item
            ingot                     *next;            //!< for list
            ingot                     *prev;            //!< for list
            
            ingot(const size_t block_size,
                  const size_t block_exp2,
                  void        *chunk_data,
                  const size_t chunk_size) throw();
            
            ~ingot() throw();
            
            bool             owns(void *block_addr, const size_t block_size) throw();
            memory::ownership whose(const void *addr)                   const throw(); //!< query ownsership
            
            void *acquire(const size_t block_size, const size_t block_exp2) throw();
            bool  release(void *block_addr,const size_t block_size, const size_t block_exp2) throw();
            
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(ingot);
        };
    }
    
}

#endif

