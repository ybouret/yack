//! \file

#ifndef YACK_MEMORY_WORKPLACE_INCLUDED
#define YACK_MEMORY_WORKPLACE_INCLUDED 1

#include "yack/type/out-of-reach.hpp"
#include "yack/arith/align.hpp"
#include <cstring>

namespace yack {

    namespace memory
    {

        //______________________________________________________________________
        //
        //
        //! workspace for internal data
        //
        //______________________________________________________________________

        template <typename T>
        class workplace
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            inline virtual     ~workplace()       throw() { eradicate(); }             //!< cleanup
            inline explicit     workplace()       throw() : words_() { eradicate(); }  //!< setup clean

            //! setup and setting address
            template <typename TARGET>
            inline explicit     workplace(TARGET * &target) throw() : words_()
            {
                eradicate();
                target = static_cast<TARGET *>( get_entry() );
            }

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            inline size_t       get_bytes() const throw() { return sizeof(words_); }                     //!< private bytes
            inline void        *get_entry()       throw() { return out_of_reach::address(&words_[0]); }  //!< entry
            inline const void  *get_entry() const throw() { return out_of_reach::address(&words_[0]); }  //!< entry, const
            inline void         eradicate() throw() { memset(words_,0,sizeof(words_)); }                 //!< cleanup content

            //! no-throw swap content
            inline void         swap_with(workplace &other) throw()
            {
                for(size_t i=0;i<sizeof(words_)/sizeof(words_[0]);++i)
                {
                    cswap(words_[i],other.words_[i]);
                }
            }

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(workplace);
            void *words_[ YACK_WORDS_FOR(T) ];
        };

    }

}

#endif
