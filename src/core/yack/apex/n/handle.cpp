
#include "yack/apex/natural.hpp"
#include "yack/system/endian.hpp"
#include <iostream>

namespace yack
{
    namespace apex
    {

        natural:: handle:: handle(const natural &n) throw() :
        count(n.words),
        entry(n.word)
        {
        }

        natural:: handle:: handle(uint_type &u) throw() :
        count(0),
        entry( natural::u2w(u,coerce(count)) )
        {
        }

        natural:: handle:: handle(word_type &W) throw() :
        count(1),
        entry(&W)
        {
            assert(W>0);
        }

        void natural::handle:: display(std::ostream &os) const
        {
            os << "#" << count << "{";
            const word_type *p = entry+count;
            for(size_t i=count;i>0;--i)
            {
                os << ' ' << std::hex << uint64_t( *(--p) );
            }
            os << " }";
        }


        bool natural::handle::is0() const throw() { return count<=0; }

        bool natural::handle::is1() const throw()
        {
            return (1==count) && (1==entry[0]);
        }


    }

}
