#include "yack/chem/balancing/frontier.hpp"

namespace yack
{
    namespace chemical
    {
        frontier:: ~frontier() throw()
        {
            coerce(xi)=0;
        }

        frontier:: frontier(const sp_fund &io) throw() :
        sp_repo(io),
        xi(0)
        {
        }


        void frontier:: destroy() throw() {
            release();
            coerce(xi) = 0;
        }

        void frontier:: reverse() throw()
        {
            coerce(xi) = -coerce(xi);
        }

        void frontier:: ldz(writable<double> & C) const throw()
        {
            for(const sp_knot *node=head;node;node=node->next)
            {
                C[ ****node ] = 0;
            }
        }

        void frontier:: mov(writable<double> &C,
                            const actor      *curr) const throw()
        {
            while(curr)
            {
                C[***curr] += xi * (curr->nu);
                curr=curr->next;
            }
        }


        std::ostream & operator<<(std::ostream &os, const frontier &b)
        {
            os << "<";
            switch(b.size)
            {
                case 0: os << "none"; break;
                default:
                    os << "xi=" << b.xi << "@" << static_cast<const sp_repo&>(b);
            }
            os << ">";
            return os;
        }

    }
}
