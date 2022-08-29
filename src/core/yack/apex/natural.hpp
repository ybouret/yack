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

        class integer;

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
            static const char clid[]; //!< "apn"
            
            typedef unsigned_for<YACK_APEX_TYPE>::type       core_type; //!< native type
            typedef unsigned_int< sizeof(core_type)/2>::type word_type; //!< internal type
            
            static const size_t core_size = sizeof(core_type);          //!< native type size
            static const size_t core_bits = sizeof(core_type) << 3;     //!< native type bits
            
            // internal storage
            static const size_t     word_size = sizeof(word_type);                //!< internal type size
            static const size_t     word_bits = sizeof(word_type) << 3;           //!< internal type bits
            static const size_t     word_exp2 = ilog2<word_size>::value;          //!< word_size = 2^word_exp2
            static const core_type  word_base = core_type(1) << word_bits;        //!< 2^[8|16|32] on larger type
            static const core_type  word_maxi = integral_for<word_type>::maximum; //!< maximum  for core_type

            // internal parameters
            static  const size_t    min_words_bytes = 2 * sizeof(uint_type);          //!< minimal memory, in bytes
            static  const size_t    min_words_size  = min_words_bytes >> word_exp2;   //!< minimal memory, in words
            static  const size_t    min_words_exp2  = ilog2<min_words_size>::value;   //!< shift for allocating
            static  const size_t    words_per_uint  = sizeof(uint_type)>>word_exp2;   //!< words per unit
            static  size_t          words_exp2_for(const size_t nw);                  //!< compute exponent such that nw <= 2^exponent

            //! number of words to use fft
            static  const size_t    shift_to_fft = 512;

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            natural();
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
            virtual size_t      serialize(ios::ostream &) const;          //!< serialize content as readable
            virtual const char *class_uid()       const throw();          //!< clid
            static  natural     construct(ios::istream &, size_t &cumul); //!< reload a serialized natural
            
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
            static natural comb(const uint_type n, const uint_type k); //!< n!/k!/(n-k)!
            
            static inline natural _fmul(const natural &u, const natural &v) { const handle U(u); const handle V(v); return fmul(U,V); } //!< call fmul
            static inline natural _lmul(const natural &u, const natural &v) { const handle U(u); const handle V(v); return lmul(U,V); } //!< call lmul
            
            static inline natural _fsqr(const natural &u) { const handle U(u); return fsqr(U);   } //!< call fsqr
            static inline natural _lsqr(const natural &u) { const handle U(u); return lmul(U,U); } //!< call lmul
            static natural        squared(const natural &); //!< squared value

            inline natural abs()  const { return *this; }          //!< |n|
            inline natural mod2() const { return squared(*this); } //!< |n|^2


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
            

            static natural gcd(const natural &a, const natural &b);                         //!< gcd
            static natural lcm(const natural &a, const natural &b);                         //!< lcm=(a*b)/gcd(a,b)
            static void    simplify(natural &num, natural &den);                            //!< simplification protocol
            static natural mod_inv(const natural &b, const natural &n);                     //!< modular inverse        (b^(-1))[n]
            static natural mod_exp(const natural &b, const natural &e, const natural &n);   //!< modular exponentiation (b^e)[n]

            //__________________________________________________________________
            //
            // bitwise ops
            //__________________________________________________________________
            //! declare some bitwise ops
#define      YACK_APN_BITWISE_DECL(OP)                                              \
/**/         friend natural operator OP (const natural  &lhs, const natural  &rhs); \
/**/         friend natural operator OP (const natural  &lhs, const uint_type rhs); \
/**/         friend natural operator OP (const uint_type lhs, const natural  &rhs); \
/**/         natural      & operator OP##=(const natural  &rhs);                    \
/**/         natural      & operator OP##=(const uint_type lhs)

            //! declare bitwise ops
#define     YACK_APN_BITWISE_OPS()         \
/**/        YACK_APN_BITWISE_DECL(|);      \
/**/        YACK_APN_BITWISE_DECL(&);      \
/**/        YACK_APN_BITWISE_DECL(^)

            YACK_APN_BITWISE_OPS();    //!< logical operators
            natural operator~() const; //!< logical

            //__________________________________________________________________
            //
            // conversion
            //__________________________________________________________________
            //! try to cast to a system integer
            template <typename T> inline
            bool try_cast_to(T &res) const throw()
            {
                uint_type    umax = uint_type( integral_for<T>::maximum );
                const handle hmax(umax);
                const handle self(*this);
                if( cmp(self,hmax) > 0 )
                    return false;
                else
                {
                    res = T( lsu() );
                    return true;
                }
            }

            //! cast to a system integer with overflow check
            template <typename T> inline
            T cast_to(const char *who=NULL) const
            {
                T res=0;
                if(!try_cast_to(res)) cast_overflow(who);
                return res;
            }



            //__________________________________________________________________
            //
            // parsing
            //__________________________________________________________________
            static natural parse_dec(const char *data,size_t size);    //!< parse decimal
            static natural parse_dec(const char *data);                //!< parse decimal
            static natural parse_hex(const char *data,size_t size);    //!< parse hexadecimal
            static natural parse_hex(const char *data);                //!< parse hexadecimal
            static natural parse(const char *data, const size_t size); //!< parse dec|hex
            static natural parse(const char *data);                    //!< parse

            //__________________________________________________________________
            //
            // floating point ops
            //__________________________________________________________________
            static double log_of(const natural &n);                      //!< log

            //__________________________________________________________________
            //
            //! integer to floating point
            //__________________________________________________________________
            template <typename T> inline T to() const
            {
                static const T fac = T(word_base);
                T              ans = 0;
                for(size_t i=words;i>0;)
                {
                    const word_type w = word[--i];
                    ans *= fac;
                    ans += w;
                }
                return ans;
            }

            //__________________________________________________________________
            //
            //! num/den = integer part + fractional part
            //__________________________________________________________________
            template <typename T> static inline
            T ratio(const natural &num, const natural &den)
            {
                static const T tenth = static_cast<T>(0.1);
                uint_type      _10   = 10;
                const handle   ten(_10);
                natural        r;
                natural        q   = quot(num,den,r);
                T              ans = q.to<float>();    // integert part
                T              fac = tenth;
                while(true)
                {
                    const handle  rh(r);
                    const natural nn = mul(rh,ten);
                    q = quot(nn,den,r);
                    const T cur = ans + fac * static_cast<T>( q.lsu() );
                    if( cur <= ans ) break;
                    fac *= tenth;
                    ans  = cur;
                }
                return ans;
            }

        private:
            friend class integer;

            size_t       bytes;          //!< current byte
            size_t       words;          //!< current words
            const size_t max_words_exp2; //!< words <= 2^max_words_exp2
            const size_t max_bytes_exp2; //!< bytes <= 2^max_bytes_exp2
            word_type   *word;           //!< least significant word address
            const size_t max_words;      //!< 1 << max_words_exp2
            const size_t max_bytes;      //!< 1 << max_bytes_exp2
            
            class handle {
            public:
                const size_t            words; //!< words count
                const size_t            bytes; //!< bytes count
                const word_type * const entry; //!< words entry
                
                explicit handle(const natural &) throw(); //!< from natural
                explicit handle(uint_type     &) throw(); //!< from uint_type
                explicit handle(word_type     &) throw(); //!< one word reference
                virtual ~handle() throw();                //!< cleanup

                std::ostream &display(std::ostream &) const; //!< helper
                bool          is0() const throw();           //!< test if is 0
                bool          is1() const throw();           //!< test if is 1
                
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
            static natural   fsqr(const handle &l);                          //!< fourier squared
            static natural   div(const  handle &numerator, const handle &denominator); //!< division
            static natural   mod(const  handle &numerator, const handle &denominator); //!< modulo
            static size_t    exp2_look_up(const handle &numerator, const handle &denominator);       //!< helper for div/mod/quot
            static natural   quot(const handle &numerator, const handle &denominator, natural &rem); //!< simultaneous quot/rem
            
            typedef uint8_t   (*bitproc)(uint8_t, uint8_t);
            static natural      bitwise(bitproc proc, const natural &lhs, const natural &rhs); //!< bitwise call

        };
        
    }
    
    typedef apex::natural apn; //!< alias
    
}


#endif
