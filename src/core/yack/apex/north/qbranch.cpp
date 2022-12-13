#include "yack/apex/north/qbranch.hpp"
#include "yack/system/exception.hpp"

namespace yack
{
    namespace north
    {
        qbranch:: qbranch()   :
        depth(0),
        qlist(),
        cache( new qidx_bank() )
        {
        }

        qbranch:: ~qbranch() throw()
        {
        }

        void qbranch:: throw_invalid_dimension() const
        {
            throw imported::exception("north::qbranch::boot","invalid new family dimension");
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
            for(const qfamily *member=qlist.head;member;member=member->next)
            {
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

