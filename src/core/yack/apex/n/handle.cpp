
#include "yack/apex/natural.hpp"
#include "yack/system/endian.hpp"
#include "yack/arith/base2.hpp"
#include "yack/ios/fmt/hexa.hpp"
#include <iostream>

namespace yack
{
    namespace apex
    {

        natural:: handle:: handle(const natural &n) noexcept :
        words(n.words),
        bytes(n.bytes),
        entry(n.word)
        {
        }

        natural:: handle:: ~handle() noexcept
        {
        }

        natural:: handle:: handle(uint_type &u) noexcept :
        words(0),
        bytes(0),
        entry( natural::u2w(u,coerce(words),coerce(bytes)) )
        {
        }

        natural:: handle:: handle(word_type &W) noexcept :
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

        std::ostream & operator << (std::ostream &os, const natural:: handle &h)
        {
            return h.display(os);
        }


        bool natural::handle::is0() const noexcept { return words<=0; }

        bool natural::handle::is1() const noexcept
        {
            return (1==words) && (1==entry[0]);
        }


    }

}
