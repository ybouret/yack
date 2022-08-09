//! \file

#ifndef YACK_IOBITS_INCLUDED
#define YACK_IOBITS_INCLUDED 1

#include "yack/ios/character.hpp"
#include "yack/data/list/cxx.hpp"
#include "yack/data/pool/cxx.hpp"
#include "yack/type/ints.hpp"

#include <iosfwd>

namespace yack
{


    typedef ios::character      io_bit;
    typedef cxx_list_of<io_bit> io_list;
    typedef cxx_pool_of<io_bit> io_pool;

    
    class io_bits : public io_list
    {
    public:
        static const io_bit::type _1 = 0x01;
        static const io_bit::type _0 = 0x00;
        static inline io_bit::type bool2type(const bool flag) throw() {
            return flag ? _1  : _0;
        }

        virtual ~io_bits() throw();
        io_bits()          throw();
        io_bits(const io_bits &other);
        void add(const io_bit::type);
        void pre(const io_bit::type);

        template <typename T> inline
        void append(const T data, size_t nbit)
        {
            typedef typename unsigned_for<T>::type utype;
            static const     utype                 one(1);
            utype word = static_cast<utype>(data);
            while(nbit-- > 0)
            {
                if(one&word) add(_1); else add(_0);
                word >>= 1;
            }
        }


        friend std::ostream & operator<<(std::ostream &os, const io_bits &Q);


        mutable io_pool pool;

    protected:
        YACK_DISABLE_ASSIGN(io_bits);
        io_bit *create(const io_bit::type);               //!< depending on pool's state
        io_bit *query_(const io_bit::type) const throw(); //!< from non-empty pool


    };
    

}

#endif

