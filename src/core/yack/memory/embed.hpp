//! \file

#ifndef YACK_MEMORY_EMBED_INCLUDED
#define YACK_MEMORY_EMBED_INCLUDED 1

#include "yack/type/out-of-reach.hpp"
#include "yack/container/as-capacity.hpp"
#include <iosfwd>

namespace yack
{
    namespace memory
    {
     
        class allocator;

        //______________________________________________________________________
        //
        //
        //! embedding (aligned) memory
        //
        //______________________________________________________________________
        class embed
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! generic setup
            template <typename T>
            inline embed( T * &addr, const size_t size) noexcept :
            handle( static_cast<void **>(out_of_reach::address(&addr)) ),
            offset(0),
            length(size*sizeof(T)),
            width_(sizeof(T))
            {
                assert(handle);
                *handle = 0;
            }

            //! setup for a raw block, using as_capacity to disambiguate
            inline embed(void *             & block_addr,
                         const size_t         block_size,
                         const as_capacity_t &) noexcept :
            handle( &block_addr ),
            offset(0),
            length( block_size ),
            width_(1)
            {

            }

            embed(const embed &) noexcept; //!< no-throw copy
            ~embed() noexcept;             //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! decrease address for C++ usage [1..something]
            void cxx() noexcept;

            //! display helper
            friend std::ostream & operator<<(std::ostream &os, const embed&);

            //! allocate all
            static void *zalloc(embed emb[], const size_t num, allocator &mem, size_t &bs);

            //! decrease all addresses
            static void  cxx(embed emb[],const size_t num) noexcept;

            

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            void       **handle; //!< pointer to address
            size_t       offset; //!< offset in memory
            size_t       length; //!< length in bytes
            const size_t width_; //!< item size

        private:
            YACK_DISABLE_ASSIGN(embed);
        };

        //! embedding for static arrays of embed[]
#define YACK_MEMORY_EMBED(EMB,MEM,BS) memory::embed::zalloc(EMB,sizeof(EMB)/sizeof(EMB[0]),MEM,BS)

        //! transforming to cxx
#define YACK_MEMORY_EMBED_TO_CXX(EMB)    memory::embed::cxx(EMB,sizeof(EMB)/sizeof(EMB[0]))

    }
}

#endif
