
#include "yack/apex/natural.hpp"
#include "yack/system/endian.hpp"
#include <iostream>

namespace yack
{
    namespace apex
    {

        natural:: handle:: handle(const natural &n) throw() :
        n(n.words),
        w(n.word)
        {
        }

        natural:: handle:: handle(uint_type &u) throw() :
        n(0),
        w( natural::u2w(u,coerce(n)) )
        {
        }

        natural:: handle:: handle(word_type &W) throw() :
        n(1),
        w(&W)
        {
            assert(W>0);
        }

        void natural::handle:: display(std::ostream &os) const
        {
            os << "#" << n << "{";
            const word_type *p = w+n;
            for(size_t i=n;i>0;--i)
            {
                os << ' ' << std::hex << uint64_t( *(--p) );
            }
            os << " }";
        }



    }

}
