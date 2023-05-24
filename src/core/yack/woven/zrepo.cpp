
#include "yack/woven/zrepo.hpp"
#include "yack/data/list/sort.hpp"

namespace yack
{
    namespace woven
    {

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

            // check no multiple
            for(const zvector *rhs=head;rhs;rhs=rhs->next)
            {
                assert(dimensions==rhs->dimensions);
                if(are_equal(lhs,*rhs)) return;
            }

            // create a new vector
            zvector &Z = * push_back( new zvector(dimensions) );
            for(size_t i=dimensions;i>0;--i)
            {
                assert(1==lhs[i].den);
                Z[i] = lhs[i].num;
            }
            Z.finalize();
        }




        std::ostream & operator<<(std::ostream &os, const zrepo &repo)
        {
            os << "<zrepo size=\'" << repo.size << "\'>" << std::endl;
            size_t i=1;
            for(const zvector *node=repo.head;node;node=node->next,++i)
            {
                os << "\tZ" << i << " = " << *node << " |" << node->weight << "| #" << node->ncoeff << std::endl;
            }
            os << "<zrepo/>";
            return os;
        }

        void zrepo:: sort()
        {
            {
                zvectors output;
                while(head)
                {
                    if(head->ncoeff<=1)
                        delete pop_front();
                    else
                        output.push_back( pop_front() );
                }
                swap_with(output);
            }
            merge_list_of<zvector>::sort(*this, zvector::compare);
        }



    }

}


