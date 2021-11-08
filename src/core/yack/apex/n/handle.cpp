
#include "yack/apex/natural.hpp"
#include "yack/system/endian.hpp"
#include "yack/arith/base2.hpp"
#include "yack/ios/fmt/hexa.hpp"
#include <iostream>

namespace yack
{
    namespace apex
    {

        natural:: handle:: handle(const natural &n) throw() :
        words(n.words),
        bytes(n.bytes),
        entry(n.word)
        {
        }

        natural:: handle:: ~handle() throw()
        {
        }

        natural:: handle:: handle(uint_type &u) throw() :
        words(0),
        bytes(0),
        entry( natural::u2w(u,coerce(words),coerce(bytes)) )
        {
        }

        natural:: handle:: handle(word_type &W) throw() :
        words(1),
        bytes(bytes_for(W)),
        entry(&W)
        {
            assert(W>0);
        }

        std::ostream & natural::handle:: display(std::ostream &os) const
        {
            os << "#" << words << ":{";
            const word_type *p = entry+words;
            for(size_t i=words;i>0;--i)
            {
                //os << ' ' << std::hex << uint64_t( *(--p) );
                os << ' ' << ios::hexa(*(--p));
            }
            os << " }";
            return os;
        }


        bool natural::handle::is0() const throw() { return words<=0; }

        bool natural::handle::is1() const throw()
        {
            return (1==words) && (1==entry[0]);
        }


    }

}
