#include "yack/chem/frontier.hpp"

namespace yack
{
    namespace chemical
    {
        frontier:: ~frontier() throw()
        {
            xi=0;
        }

        frontier:: frontier(const sp_fund &io) throw() :
        sp_repo(io),
        xi(0)
        {
        }


        void frontier:: destroy() throw() {
            release();
            xi = 0;
        }


        std::ostream & operator<<(std::ostream &os, const frontier &b)
        {
            os << "[";
            switch(b.size)
            {
                case 0: os << "none"; break;
                default:
                    os << "xi=" << b.xi << "@" << static_cast<const sp_repo&>(b);
            }
            os << "]";
            return os;
        }

    }
}
