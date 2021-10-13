
//! \file

#ifndef YACK_APEX_NATURAL_INCLUDED
#define YACK_APEX_NATURAL_INCLUDED 1

#include "yack/apex/number.hpp"
#include "yack/apex/types.hpp"
#include "yack/arith/ilog2.hpp"
#include <iosfwd>

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
            static  const size_t    word_size = cull::word_size;          //!< alias
            static  const size_t    word_exp2 = ilog2<word_size>::value;  //!< word_size = 2^word_exp2
            static  const size_t    word_bits = cull::word_bits;          //!< word_size*8

            static  const size_t    min_words_bytes = 2 * sizeof(unsigned_type);           //!< minimal memory, in bytes
            static  const size_t    min_words_size  = min_words_bytes / sizeof(word_type); //!< minimal memory, in words
            static  const size_t    min_words_exp2  = ilog2<min_words_size>::value;        //!< shift for allocating

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            natural(unsigned_type u);                          //!< setup with an integral type
            virtual ~natural() throw();                        //!< cleanup
            natural(const natural &);                          //!< copy
            natural & operator= (const natural &);             //!< assign
            natural & operator= (const unsigned_type) throw(); //!< assign

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            size_t        size()        const throw(); //!< bytes which are not zero
            size_t        bits()        const throw(); //!< exact bits
            void          xch(natural &other) throw(); //!< no throw exchange all fields
            unsigned_type lsu()         const throw(); //!< least significant unsigned
            void          ldu(unsigned_type)  throw(); //!< load unsigned type
            void          ldz()               throw(); //!< load zero

            //! access operator
            const uint8_t & operator[](const size_t indx) const throw();

            //! output
            friend std::ostream & operator<<(std::ostream &os, const natural n);
            std::ostream & output_hex(std::ostream &) const;


        private:
            size_t       words;
            size_t       bytes;
            const int    max_words_exp2;
            const int    max_bytes_exp2;
            word_type   *word;
            const size_t max_words; //!< 1 << wexp2
            const size_t max_bytes; //!< 1 << block_exp2
            
            void update() throw();
            void zpad()   throw();
        };

    }

    typedef apex::natural apn; //!< alias

}


#endif
