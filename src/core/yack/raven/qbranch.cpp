#include "yack/raven/qbranch.hpp"

namespace yack
{
    namespace raven
    {

        qbranch:: ~qbranch() noexcept
        {
            prune();
        }

        void qbranch:: prune() noexcept
        {
            qlist.release();
        }

        qbranch:: qbranch():
        qlist(),
        io( new qBank() )
        {
        }

        const list_of<qfamily> & qbranch:: operator*()  const noexcept
        {
            return qlist;
        }

        const list_of<qfamily> * qbranch:: operator->()  const noexcept
        {
            return &qlist;
        }

        std::ostream & operator<<(std::ostream &os, const qbranch &self)
        {
            os << " |#" << self.qlist.size << "|={" << std::endl;
            for(const qfamily *member = self->head; member; member=member->next)
            {
                os << "\t" << *member << std::endl;
            }
            os << "}";
            return os;
        }

    }

}


