
//! \file

#ifndef YACK_APEX_NATURAL_INCLUDED
#define YACK_APEX_NATURAL_INCLUDED 1

#include "yack/apex/number.hpp"
#include "yack/apex/types.hpp"
#include "yack/arith/ilog2.hpp"
#include "yack/container/readable.hpp"
#include "yack/container/as-capacity.hpp"
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
        class natural : public number, public readable<uint8_t>
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef cull::core_type core_type; //!< alias

            typedef cull::word_type word_type;                            //!< alias
            static  const size_t    word_size = cull::word_size;          //!< alias
            static  const size_t    word_exp2 = ilog2<word_size>::value;  //!< word_size = 2^word_exp2
            static  const size_t    word_bits = cull::word_bits;          //!< word_size*8

            static  const size_t    min_words_bytes = 2 * sizeof(uint_type);          //!< minimal memory, in bytes
            static  const size_t    min_words_size  = min_words_bytes >> word_exp2;   //!< minimal memory, in words
            static  const int       min_words_exp2  = ilog2<min_words_size>::value;   //!< shift for allocating
            static  const size_t    words_per_uint  = sizeof(uint_type)>>word_exp2;   //!< words per unit
            static  int             words_exp2_for(const size_t nw);                  //!< compute exponent such that nw <= 2^exponent

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            natural(uint_type u);                              //!< setup with an integral type
            virtual ~natural() throw();                        //!< cleanup
            natural(const natural &);                          //!< copy
            natural & operator= (const natural &);             //!< assign
            natural & operator= (const uint_type) throw();     //!< assign

            //__________________________________________________________________
            //
            // readable interface
            //__________________________________________________________________
            virtual size_t          size()                   const throw(); //!< significant bytes
            virtual const uint8_t & operator[](const size_t) const throw(); //!< access operator

            //__________________________________________________________________
            //
            // read/write methods
            //__________________________________________________________________
            size_t        bits()        const throw(); //!< exact bits
            void          xch(natural &other) throw(); //!< no throw exchange all fields
            uint_type     lsu()         const throw(); //!< least significant unsigned
            void          ldu(uint_type)      throw(); //!< load unsigned type
            void          ldz()               throw(); //!< load zero


            //! output
            friend std::ostream & operator<<(std::ostream &os, const natural n);

            //! output hexadecimal, compact form
            std::ostream & output_hex(std::ostream &) const;

            //! convert value to array of words, uses value as memory
            static const word_type *u2w(uint_type &value, size_t &num_words) throw();

            //__________________________________________________________________
            //
            // comparisons
            //__________________________________________________________________
#define     YACK_APN_BINARY_IMPL(PROTO) const natural::handle l(lhs), r(rhs); return PROTO(l,r)

#define     YACK_APN_BINARY_REP(PROLOG,EPILOG)         \
PROLOG (const natural &lhs, const natural &rhs) EPILOG \
PROLOG (const natural &lhs, uint_type      rhs) EPILOG \
PROLOG (uint_type      lhs, const natural &rhs) EPILOG

            YACK_APN_BINARY_REP(int compare,const throw();)

#define YACK_APN_COMPARE(OP) \
YACK_APN_BINARY_REP(inline friend bool operator OP, throw() { YACK_APN_BINARY_IMPL(cmp) OP 0;})


            YACK_APN_COMPARE(<)
            YACK_APN_COMPARE(<=)
            YACK_APN_COMPARE(==)
            YACK_APN_COMPARE(!=)
            YACK_APN_COMPARE(>=)
            YACK_APN_COMPARE(>)


            //__________________________________________________________________
            //
            // addition
            //__________________________________________________________________
#define YACK_APN_UNARY_IMPL(OP)    natural res = (*this) OP rhs; xch(res); return *this
#define YACK_APN_UNARY_REP(PROLOG,EPILOG) \
PROLOG (const natural &rhs) EPILOG        \
PROLOG (uint_type      rhs) EPILOG

#define YACK_APN_BINARY_DECL(OP) YACK_APN_BINARY_REP(friend natural operator OP,;)
#define YACK_APN_UNARY_DECL(OP)  YACK_APN_UNARY_REP(natural & operator OP##=,;)
#define YACK_APN_DECL(OP) YACK_APN_BINARY_DECL(OP) YACK_APN_UNARY_DECL(OP)

            YACK_APN_DECL(+)
            natural &operator++();    //!< pre  increase operator
            natural  operator++(int); //!< post increase operator


        private:
            size_t       words;
            size_t       bytes;
            const int    max_words_exp2;
            const int    max_bytes_exp2;
            word_type   *word;
            const size_t max_words; //!< 1 << wexp2
            const size_t max_bytes; //!< 1 << block_exp2

            class handle {
            public:
                const size_t            n;
                const word_type * const w;

                handle(const natural &) throw();
                handle(uint_type     &) throw();
                handle(word_type     &) throw();

                void display(std::ostream &) const;

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(handle);
            };


            void update() throw(); //!< from words and bytes
            void zpad()   throw(); //!< after words to max_words
            natural(const size_t num_words, const as_capacity_t &);  //!< capacity for num_words
            natural(const word_type *w, const size_t n);

            static size_t  ldw(word_type *,uint_type) throw(); //!< load uint into word[words_per_uint], return num words

            static int     cmp(const handle &l,
                               const handle &r) throw();

            static natural add(const handle &l,
                               const handle &r);

        };

    }

    typedef apex::natural apn; //!< alias

}


#endif
