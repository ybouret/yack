#include "yack/raven/qfamily.hpp"


namespace yack
{
    namespace raven
    {

        qfamily:: ~qfamily() throw()
        {
        }

        qmatrix       & qfamily:: operator*()       throw()
        {
            return *qbase;
        }

        const qmatrix & qfamily:: operator*() const throw()
        {
            return *qbase;
        }

        std::ostream & operator<<(std::ostream &os, const qfamily &self)
        {
            os << *(self.qbase) << "@" << *(self.basis) << "+" << *(self.ready);
            return os;
        }


        qfamily:: qfamily(const qfamily &F) :
        object(),
        qbase( F.qbase ),
        basis( F.basis ),
        ready( F.ready ),
        next(0),
        prev(0)
        {
        }

       



    }

}

