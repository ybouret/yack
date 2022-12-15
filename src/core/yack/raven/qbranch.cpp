#include "yack/raven/qbranch.hpp"

namespace yack
{
    namespace raven
    {

        qbranch:: ~qbranch() throw()
        {
            prune();
        }

        void qbranch:: prune() throw()
        {
            qlist.release();
            coerce(depth) = 0;
        }

        qbranch:: qbranch():
        depth(),
        qlist(),
        io( new qBank() )
        {
        }

        const list_of<qfamily> & qbranch:: operator*()  const throw()
        {
            return qlist;
        }

        const list_of<qfamily> * qbranch:: operator->()  const throw()
        {
            return &qlist;
        }

        std::ostream & operator<<(std::ostream &os, const qbranch &self)
        {
            os << "{" << std::endl;
            for(const qfamily *member = self->head; member; member=member->next)
            {
                os << "\t" << *member << std::endl;
            }
            os << "}";
            return os;
        }

    }

}


