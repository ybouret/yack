//! \file

#ifndef YACK_GFX_BITFIELD_INCLUDED
#define YACK_GFX_BITFIELD_INCLUDED 1

#include "yack/gfx/article.hpp"
#include "yack/memory/operative.hpp"
#include "yack/arith/align.hpp"
#include "yack/type/out-of-reach.hpp"

namespace yack
{
    namespace graphic
    {

        //______________________________________________________________________
        //
        //
        //! shared memory for (bit|pix)maps
        //
        //______________________________________________________________________
        class bitfield : public article
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef memory::operative io_type; //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit bitfield(const size_t n); //!< setup bytes>=n
            virtual ~bitfield() throw();       //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! fill with items
            template <typename T>
            void fill(const size_t items) throw()
            {
                assert(NULL==memio);
                assert(items*sizeof(T)<=bytes);
                memio = new( YACK_STATIC_ZSET(wksp) ) memory::operative_of<T>(entry,items);
            }

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const size_t   bytes; //!< used bytes
            void          *entry; //!< first byte address

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(bitfield);
            typedef memory::operative_of<uint8_t> io_type_;
            io_type *memio;
            void    *wksp[ YACK_WORDS_FOR(io_type_) ];
        };

    }

}

#endif
