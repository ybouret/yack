
#include "yack/woven/qrepository.hpp"

namespace yack
{
    namespace woven
    {

        zvector:: zvector(const size_t dims) :
        metrics(dims),
        ztableau(dims)
        {
        }

        zvector:: ~zvector() noexcept
        {
        }


        zvector:: zvector(const zvector &other) : metrics(other), ztableau(other)
        {
        }
        


        zrepository:: ~zrepository() noexcept {}

        zrepository:: zrepository(const size_t dims)  : metrics(dims), zvectors()
        {
        }

        zrepository:: zrepository(const zrepository &other) : metrics(other), zvectors(other)
        {

        }


        static inline bool are_equal(const qvector &lhs, const zvector &rhs) noexcept
        {
            assert(lhs.size()==rhs.size());
            for(size_t i=lhs.size();i>0;--i)
            {
                if(lhs[i].num!=rhs[i]) return false;
            }
            return true;
        }

        void zrepository:: ensure(const qvector &lhs)
        {
            assert(dimensions==lhs.dimensions);
            for(const zvector *rhs=head;rhs;rhs=rhs->next)
            {
                assert(dimensions==rhs->dimensions);
                if(are_equal(lhs,*rhs)) return;
            }

            //! create a new vector
            zvector &Z = * push_back( new zvector(dimensions) );
            for(size_t i=dimensions;i>0;--i)
            {
                assert(1==lhs[i].den);
                Z[i] = lhs[i].num;
            }
        }


        void zrepository:: ensure(const list_of<qvector> &lhs)
        {
            for(const qvector *qv=lhs.head;qv;qv=qv->next)
            {
                ensure(*qv);
            }
        }
    }

}


