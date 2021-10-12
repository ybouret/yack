
//! \file

#ifndef YACK_APEX_NATURAL_INCLUDED
#define YACK_APEX_NATURAL_INCLUDED 1

#include "yack/apex/number.hpp"
#include "yack/apex/types.hpp"
#include "yack/arith/ilog2.hpp"

namespace yack
{

    namespace apex
    {

        //______________________________________________________________________
        //
        //
        //! drop-in unsigned replacement
        //
        //______________________________________________________________________
        class natural : public number
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef cull::word_type word_type; //!< alias
            static  const size_t    word_size = cull::word_size;
            static  const size_t    word_exp2 = ilog2<word_size>::value;
            static  const size_t    word_bits = cull::word_bits;
            static  const size_t    min_words_bytes = 2 * sizeof(unsigned_type);
            static  const size_t    min_words_size  = min_words_bytes / sizeof(word_type);
            static  const size_t    min_words_exp2  = ilog2<min_words_size>::value;

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            natural(unsigned_type u);               //!< setup with an integral type
            virtual ~natural() throw();             //!< cleanup
            natural(const natural &);               //!< copy
            natural & operator= (const natural &);  //!< assign

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            size_t        size() const throw();        //!< bytes which are not zero
            size_t        bits() const throw();        //!< exact bits
            void          xch(natural &other) throw(); //!< no throw exchange all fields
            unsigned_type lsu() const throw();         //!< least significant unsigned


        private:
            size_t       bytes;
            size_t       words;
            const int    words_exp2;
            const int    block_exp2;
            word_type   *word;
            const size_t words_size; //!< 1 << words_exp2
            const size_t block_size; //!< 1 << block_exp2

            void update() throw();
        };

    }

    typedef apex::natural apn; //!< alias

}


#endif
