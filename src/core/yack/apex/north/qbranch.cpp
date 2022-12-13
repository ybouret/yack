#include "yack/apex/north/qbranch.hpp"

namespace yack
{
    namespace north
    {
        qbranch:: qbranch()   :
        depth(0),
        qlist(),
        idxIO( new qidx_bank() )
        {
        }

        qbranch:: ~qbranch() throw()
        {
        }

        std::ostream & operator<<(std::ostream &os, const qbranch &self)
        {
            os << "|#" << self.qlist.size << "@depth=" << self.depth << "| {" << std::endl;
            for(const qfamily *f=self.qlist.head;f;f=f->next)
            {
                os << "\t" << *f << std::endl;
            }
            os << "}";
            return os;
        }

        void qbranch::prune() throw()
        {
            qlist.release();
            coerce(depth) = 0;
        }

        bool qbranch:: check_depth() const throw()
        {
            const size_t dims = qlist.head ? qlist.head->qbase->dimension : 0;
            for(const qfamily *member=qlist.head;member;member=member->next)
            {
                assert((**member).dimension==dims);
                if(depth!=member->qbase->size()) return false;
            }
            return true;
        }

        bool qbranch:: found_twins() const throw()
        {
            for(const qfamily *i=qlist.head;i;i=i->next)
            {
                const qmatrix &I = **i;
                for(const qfamily *j=i->next;j;j=j->next)
                {
                    if(I == **j) return true;
                }
            }
            return false;
        }

        
    }

}

