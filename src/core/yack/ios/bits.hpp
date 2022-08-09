//! \file

#ifndef YACK_IOBITS_INCLUDED
#define YACK_IOBITS_INCLUDED 1

#include "yack/ios/character.hpp"
#include "yack/data/list/cxx.hpp"
#include "yack/data/pool/cxx.hpp"
#include "yack/type/ints.hpp"
#include "yack/ios/ostream.hpp"
#include "yack/ios/istream.hpp"

#include <iosfwd>

namespace yack
{

    namespace apex { class natural;     }
    namespace randomized { class bits;  }

    //__________________________________________________________________________
    //
    //
    // types for io_bits
    //
    //__________________________________________________________________________
    typedef ios::character      io_bit;  //!< alias to share ios::characters
    typedef cxx_list_of<io_bit> io_list; //!< alias
    typedef cxx_pool_of<io_bit> io_pool; //!< alias


    //__________________________________________________________________________
    //
    //
    //! list of bits and operations
    //
    //__________________________________________________________________________
    class io_bits : public io_list, public ios::ostream, public ios::istream
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        static const io_bit::type _1 = 0x01; //!< alias
        static const io_bit::type _0 = 0x00; //!< alias
        static const unsigned     npad[8];   //!< number of padding bits to reach aligned size

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        virtual ~io_bits() throw();    //!< cleanup
        io_bits()          throw();    //!< setup empty
        io_bits(const io_bits &other); //!< copy using other pool if possible
        void add(const io_bit::type);  //!< append  new bit
        void pre(const io_bit::type);  //!< prepend new bit

        void          add(const apex::natural &); //!< add apn
        apex::natural ap(size_t nbit);            //!< pluck apn

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        virtual void release() throw(); //!< list into pool

        //______________________________________________________________________
        //
        // ostream interface
        //______________________________________________________________________
        virtual void write(const char C);       //!< write 8 bits
        virtual void flush();                   //!< align to next 8 bits boundary
        void         rfill(randomized::bits &); //!< align to next 8 bits boundary with random

        //______________________________________________________________________
        //
        // generic methods
        //______________________________________________________________________

        //! little endian append
        template <typename T> inline void append(const T data, size_t nbit)
        {
            assert(nbit<=sizeof(T)*8);
            typedef typename unsigned_for<T>::type utype;
            static const     utype                 one(1);
            
            utype       mask = one << (nbit-1);
            const utype word = static_cast<utype>(data);
            while(nbit-- > 0)
            {
                if(mask&word) add(_1); else add(_0);
                mask >>= 1;
            }
        }

        //! append full word
        template <typename T> inline
        void append(const T data)
        {
            typedef typename unsigned_for<T>::type utype;
            static  const     size_t               ubits = sizeof(utype) * 8;
            static const      utype                one(1);

            utype       mask = one << (ubits-1);
            const utype word = static_cast<utype>(data);
            size_t      nbit = ubits;
            while(nbit-- > 0)
            {
                if(mask&word) add(_1); else add(_0);
                mask >>= 1;
            }
        }

        //! pluck bits to form a word
        template <typename T> inline T pluck(size_t nbit) throw()
        {
            assert(nbit<=sizeof(T)*8);
            assert(size>=nbit);

            typedef typename unsigned_for<T>::type utype;
            static const     utype                 one(1);

            utype word = 0;
            while(nbit-- > 0)
            {
                word <<= 1;
                if( **pool.store(pop_front())  )
                {
                    word |= one;
                }
            }
            return static_cast<T>(word);
        }

        //! pluck a full word
        template <typename T> inline T pluck() throw()
        {
            return pluck<T>(sizeof(T)*8);
        }


        //! display helper
        friend std::ostream & operator<<(std::ostream &os, const io_bits &Q);

        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
        mutable io_pool pool; //!< used a level-2 cache

    private:
        YACK_DISABLE_ASSIGN(io_bits);
        io_bit *make(const io_bit::type);               //!< depending on pool's state
        io_bit *fast(const io_bit::type) const throw(); //!< from non-empty pool

        virtual bool   query_(char &C) throw();
        virtual size_t fetch_(void *addr, size_t size) throw();

    };
    

}

#endif

