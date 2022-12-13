

#include "yack/apex/north/qbranch.hpp"

namespace yack
{
    namespace north
    {
        qbranch:: qbranch() throw() : qfamily::list_type()
        {
        }

        qbranch:: ~qbranch() throw()
        {
        }

        std::ostream & operator<<(std::ostream &os, const qbranch &self)
        {
            os << "{" << std::endl;
            for(const qfamily *f=self.head;f;f=f->next)
            {
                os << "\t" << *f << std::endl;
            }
            os << "}";
            return os;
        }

    }

}

