
#include "yack/woven/zrepo.hpp"
#include "yack/data/list/sort.hpp"

namespace yack
{
    namespace woven
    {

        zvector:: zvector(const size_t dims) :
        object(),
        metrics(dims),
        ztableau(dims),
        next(0),
        prev(0)
        {
        }

        zvector:: ~zvector() noexcept
        {
        }


        zvector:: zvector(const zvector &other) :
        collection(),
        object(),
        metrics(other),
        ztableau(other),
        next(0),
        prev(0)
        {
        }
        
        int zvector:: compare(const zvector *lhs, const zvector *rhs) noexcept
        {
            assert(lhs->size()==rhs->size());
            return comparison::lexicographic(&(*lhs)[1], &(*rhs)[1], rhs->size());
        }

        zrepo:: ~zrepo() noexcept {}

        zrepo:: zrepo(const size_t dims)  : metrics(dims), zvectors()
        {
        }

        zrepo:: zrepo(const zrepo &other) : metrics(other), zvectors(other)
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

        void zrepo:: ensure(const qvector &lhs)
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
            std::cerr << "added " << Z << std::endl;
        }


        void zrepo:: ensure(const list_of<qvector> &lhs)
        {
            for(const qvector *qv=lhs.head;qv;qv=qv->next)
            {
                ensure(*qv);
            }
        }

        std::ostream & operator<<(std::ostream &os, const zrepo &repo)
        {
            os << "<zrepo size=\'" << repo.size << "\'>" << std::endl;
            size_t i=1;
            for(const zvector *node=repo.head;node;node=node->next,++i)
            {
                os << "\tZ" << i << " = " << *node << std::endl;
            }
            os << "<zrepo/>";
            return os;
        }

        void zrepo:: sort()
        {
            merge_list_of<zvector>::sort(*this, zvector::compare);
        }



    }

}


