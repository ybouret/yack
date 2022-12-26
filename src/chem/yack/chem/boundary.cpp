#include "yack/chem/boundary.hpp"

namespace yack
{
    namespace chemical
    {

        boundary:: ~boundary() throw()
        {
        }

        boundary:: boundary(const sp_fund &io) throw() :
        frontier(io)
        {
        }

        

    }

}

namespace yack
{
    namespace chemical
    {

        boundaries:: ~boundaries() throw()
        {
        }

        boundaries:: boundaries(const size_t m, const sp_fund &io):
        readable<boundary>(),
        bnd(m),
        bio(io)
        {
        }

        size_t boundaries:: size() const throw() { return bnd.size(); }

        const boundary & boundaries:: operator[](const size_t i) const throw()
        {
            assert(i>=1);
            assert(i<=bnd.size());
            return bnd[i];
        }


        void boundaries:: destroy() throw()
        {
            bnd.free();
        }

        void boundaries:: upgrade(const double x, const species &s)
        {
            assert(x>=0);
            if(bnd.size()<=0)
            {
                boundary &b = bnd.add<const sp_fund&>(bio);
                b.xi = x;
                b << &s;
            }
            else
            {
                exit(0);
            }
        }

    }

}

