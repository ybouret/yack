//! \file

#ifndef YACK_MEMORY_WORKPLACE_INCLUDED
#define YACK_MEMORY_WORKPLACE_INCLUDED 1

#include "yack/type/out-of-reach.hpp"
#include "yack/arith/align.hpp"
#include "yack/check/static.hpp"
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
            // definitions
            //__________________________________________________________________
            static const size_t num_words = YACK_WORDS_FOR(T);          //!< words to hold type
            static const size_t num_bytes = num_words * sizeof(void*);  //!< bytes to hold typed
            
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            inline virtual     ~workplace()       throw() { eradicate(); }             //!< cleanup
            inline explicit     workplace()       throw() : words_()
            {
                YACK_STATIC_CHECK(sizeof(words_)>=sizeof(T),bad_size);
                assert(words_==out_of_reach::address(&words_[0]));
                eradicate();
            }  //!< setup clean

            //! setup and setting address
            template <typename TARGET>
            inline explicit     workplace(TARGET * &target) throw() : words_()
            {
                eradicate();
                target = static_cast<TARGET *>( **this );
            }

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            inline void        * operator*()       throw() { return out_of_reach::address(&words_[0]); }  //!< entry
            inline const void  * operator*() const throw() { return out_of_reach::address(&words_[0]); }  //!< entry, const
            inline void          eradicate()       throw() { memset(words_,0,sizeof(words_)); }           //!< cleanup content

            //! no-throw swap content
            inline void         swap_with(workplace &other) throw() {
                for(size_t i=0;i<num_words;++i)
                    cswap(words_[i],other.words_[i]);
            }

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(workplace);
            void *words_[num_words];
        };

    }

}

#endif

