
//! \file

#ifndef YACK_APEX_NATURAL_INCLUDED
#define YACK_APEX_NATURAL_INCLUDED 1

#include "yack/apex/number.hpp"
#include "yack/apex/types.hpp"
#include "yack/arith/ilog2.hpp"
#include "yack/container/readable.hpp"
#include "yack/container/as-capacity.hpp"
#include "yack/ios/fwd.hpp"
#include <iosfwd>

namespace yack
{

    namespace randomized
    {
        class bits;
    }

    namespace apex
    {


        //#define YACK_APEX_TYPE uint16_t
        //#define YACK_APEX_TYPE uint32_t

#if !defined(YACK_APEX_TYPE)
#     define YACK_APEX_TYPE void * //!< default matching type
#endif

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

            // native computation
            typedef unsigned_for<YACK_APEX_TYPE>::type       core_type; //!< native type
            typedef unsigned_int< sizeof(core_type)/2>::type word_type;      //!< internal type

            static const size_t core_size = sizeof(core_type);      //!< native type size
            static const size_t core_bits = sizeof(core_type) << 3; //!< native type bits

            // internal storage
            static const size_t     word_size = sizeof(word_type);                   //!< internal type size
            static const size_t     word_bits = sizeof(word_type) << 3;                   //!< internal type bits
            static const size_t     word_exp2 = ilog2<word_size>::value;          //!< word_size = 2^word_exp2
            static const core_type  word_base = core_type(1) << word_bits;        //!< 2^[8|16|32] on larger type
            static const core_type  word_maxi = integral_for<word_type>::maximum; //!< maximum  for core_type
            

            static  const size_t    min_words_bytes = 2 * sizeof(uint_type);          //!< minimal memory, in bytes
            static  const size_t    min_words_size  = min_words_bytes >> word_exp2;   //!< minimal memory, in words
            static  const size_t    min_words_exp2  = ilog2<min_words_size>::value;   //!< shift for allocating
            static  const size_t    words_per_uint  = sizeof(uint_type)>>word_exp2;   //!< words per unit
            static  size_t          words_exp2_for(const size_t nw);                  //!< compute exponent such that nw <= 2^exponent
                                                                                      
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            natural(uint_type u);                               //!< setup with an integral type
            virtual ~natural() throw();                         //!< cleanup
            natural(const natural &);                           //!< copy
            natural & operator= (const natural &);              //!< assign
            natural & operator= (const uint_type) throw();      //!< assign
            natural(randomized::bits &ran, const size_t nbits); //!< make exactly nbits, and random

            //__________________________________________________________________
            //
            // readable interface
            //__________________________________________________________________
            virtual size_t          size()                   const throw(); //!< significant bytes
            virtual const uint8_t & operator[](const size_t) const throw(); //!< access operator

            //__________________________________________________________________
            //
            // serializable interface
            //__________________________________________________________________
            virtual size_t  serialize(ios::ostream &) const;
            static  natural construct(ios::istream &, size_t &cumul);

            //__________________________________________________________________
            //
            // read/write methods
            //__________________________________________________________________
            size_t        bits()           const throw(); //!< exact bits
            void          xch(natural &other)    throw(); //!< no throw exchange all fields
            uint_type     lsu()            const throw(); //!< least significant unsigned
            void          ldu(uint_type)         throw(); //!< load unsigned type
            void          ldz()                  throw(); //!< load zero
            bool          bit(size_t ibit) const throw(); //!< access 0<=ibit<bits()

            //! output
            friend std::ostream & operator<<(std::ostream &os, const natural n);

            //! output hexadecimal, compact form
            std::ostream & output_hex(std::ostream &) const;

            //! output binary, compact form
            std::ostream & output_bin(std::ostream &) const;

            //! output decimal
            std::ostream & output_dec(std::ostream &) const;


            //! convert value to array of words, uses value as memory
            static const word_type *u2w(uint_type &value,
                                        size_t    &num_words,
                                        size_t    &num_bytes) throw();

            //__________________________________________________________________
            //
            // comparisons
            //__________________________________________________________________

            //! make local handles and call PROTO
#define     YACK_APN_BINARY_IMPL(PROTO) const natural::handle l(lhs), r(rhs); return PROTO(l,r)

            //! repeat binary pattern
#define     YACK_APN_BINARY_REP(PROLOG,EPILOG)                       \
/**/          PROLOG (const natural &lhs, const natural &rhs) EPILOG \
/**/          PROLOG (const natural &lhs, uint_type      rhs) EPILOG \
/**/          PROLOG (uint_type      lhs, const natural &rhs) EPILOG


            YACK_APN_BINARY_REP(static int compare, throw();)

            //! declare and inline implement a comparison operator
#define     YACK_APN_COMPARE(OP) YACK_APN_BINARY_REP(inline friend bool operator OP, throw() { YACK_APN_BINARY_IMPL(cmp) OP 0;})


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

            //! implement a binary to unary transform
#define     YACK_APN_UNARY_IMPL(OP)    natural res = (*this) OP rhs; xch(res); return *this

            //! repeat unary pattern
#define     YACK_APN_UNARY_REP(PROLOG,EPILOG)           \
/**/          PROLOG (const natural &rhs) EPILOG        \
/**/          PROLOG (uint_type      rhs) EPILOG

#define       YACK_APN_BINARY_DECL(OP) YACK_APN_BINARY_REP(friend natural operator OP,;) //!< declare from a binary operator
#define       YACK_APN_UNARY_DECL(OP)  YACK_APN_UNARY_REP(natural & operator OP##=,;)    //!< declare from a unary operator
#define       YACK_APN_DECL(OP) YACK_APN_BINARY_DECL(OP) YACK_APN_UNARY_DECL(OP)         //!< declare all

            YACK_APN_DECL(+)
            natural &operator++();      //!< pre  increase operator
            natural  operator++(int);   //!< post increase operator
            natural  operator+() const; //!< unary+

            //__________________________________________________________________
            //
            // subtraction
            //__________________________________________________________________
            YACK_APN_DECL(-)
            natural &operator--();    //!< pre  decrease operator
            natural  operator--(int); //!< post decrease operator


            //__________________________________________________________________
            //
            // multiplication
            //__________________________________________________________________
            YACK_APN_DECL(*)
            static natural factorial(uint_type n); //!< n!
        
            static inline natural _fmul(const natural &u, const natural &v) { const handle U(u); const handle V(v); return fmul(U,V); } //!< call fmul
            static inline natural _lmul(const natural &u, const natural &v) { const handle U(u); const handle V(v); return lmul(U,V); } //!< call lmul

            //__________________________________________________________________
            //
            // power of two and bits shifting
            //__________________________________________________________________
            static natural exp2(const size_t shift);       //!< 2^shift
            natural        &shr() throw();                 //!< optimized one bit right shift, aka /=2
            natural         shr(const size_t shift) const; //!<  shift right
            natural         shl(const size_t shift) const; //!< shift left
            friend natural  operator<<(const natural &lhs, const size_t shift); //!< shift left
            friend natural  operator>>(const natural &lhs, const size_t shift); //!< shift right
            natural &       operator<<=(const size_t shift); //!< shift left
            natural &       operator>>=(const size_t shift); //!< shift right

            //__________________________________________________________________
            //
            // division
            //__________________________________________________________________
            YACK_APN_DECL(/)


            //__________________________________________________________________
            //
            // modulo
            //__________________________________________________________________
            YACK_APN_DECL(%)

            //__________________________________________________________________
            //
            // arithmetic
            //__________________________________________________________________

            //! numerator = quot * denominator + rem
            static natural quot(const natural &numerator, const natural &denominator, natural &rem);

            //! gcd
            static natural gcd(const natural &a, const natural &b);

            //! simplification
            static void simplify(natural &num, natural &den);

        private:
            size_t       bytes;
            size_t       words;
            const size_t max_words_exp2;
            const size_t max_bytes_exp2;
            word_type   *word;
            const size_t max_words; //!< 1 << wexp2
            const size_t max_bytes; //!< 1 << block_exp2

            class handle {
            public:
                const size_t            words; //!< words count
                const size_t            bytes; //!< bytes count
                const word_type * const entry; //!< words entry

                handle(const natural &) throw();
                handle(uint_type     &) throw();
                handle(word_type     &) throw();

                void display(std::ostream &) const;
                bool is0() const throw();
                bool is1() const throw();

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(handle);
            };


            void     update() throw(); //!< from words and bytes
            void     zpad()   throw(); //!< after words to max_words
            explicit natural(const size_t num_words, const as_capacity_t &);  //!< capacity for num_words
            explicit natural(const word_type *w, const size_t n);             //!< from words
            void     set_bit(const size_t ibit) throw();

            static size_t    ldw(word_type *,
                                 uint_type,
                                 size_t  &) throw(); //!< load uint into word[words_per_uint], return num words

            static int       cmp(const  handle &l, const handle &r) throw(); //!< comparison
            static sign_type scmp(const handle &l, const handle &r) throw(); //!< comparions to sign_type
            static natural   add(const  handle &l, const handle &r);         //!< addition
            static natural   sub(const  handle &l, const handle &r);         //!< subtraction
            static natural   mul(const  handle &l, const handle &r);         //!< multiplication => lmul/fmul
            static natural   lmul(const handle &l, const handle &r);         //!< long multiplication
            static natural   fmul(const handle &l, const handle &r);         //!< fourier multiplication
            static natural   div(const  handle &numerator, const handle &denominator); //!< division
            static natural   mod(const  handle &numerator, const handle &denominator); //!< modulo
            static size_t    exp2_look_up(const handle &numerator, const handle &denominator);       //!< helper for div/mod/quot
            static natural   quot(const handle &numerator, const handle &denominator, natural &rem); //!< simultaneous quot/rem

        };

    }

    typedef apex::natural apn; //!< alias

}


#endif
