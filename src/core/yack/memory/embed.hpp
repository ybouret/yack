//! \file

#ifndef YACK_MEMORY_EMBED_INCLUDED
#define YACK_MEMORY_EMBED_INCLUDED 1

#include "yack/setup.hpp"
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
            ~embed() throw(); //!< cleanup
            
            void       **handle; //!< pointer to address
            size_t       offset; //!< offset in memory
            size_t       length; //!< length in bytes
            const size_t width_; //!< item size

            //! generic setup
            template <typename T>
            inline embed( T * &addr, const size_t size) throw() :
            handle( (void **)&addr ),
            offset(0),
            length(size*sizeof(T)),
            width_(sizeof(T))
            {
                assert(handle);
                *handle = 0;
            }

            //! no-throw copy
            embed(const embed &) throw();

            //! display helper
            friend std::ostream & operator<<(std::ostream &os, const embed&);

            //! allocate all
            static void *zalloc(embed emb[], const size_t num, allocator &mem, size_t &bs);
            
        private:
            YACK_DISABLE_ASSIGN(embed);
        };

        //! embedding for static arrays of embed[]
#define YACK_MEMORY_EMBED(EMB,MEM,BS) memory::embed::zalloc(EMB,sizeof(EMB)/sizeof(EMB[0]),MEM,BS)
    }
}

#endif
