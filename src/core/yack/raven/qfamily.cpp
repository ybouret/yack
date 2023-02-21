#include "yack/raven/qfamily.hpp"


namespace yack
{
    namespace raven
    {

        qfamily:: ~qfamily() noexcept
        {
        }

        qmatrix       & qfamily:: operator*()       noexcept
        {
            return *qbase;
        }

        const qmatrix & qfamily:: operator*() const noexcept
        {
            return *qbase;
        }

        qmatrix       * qfamily:: operator->()       noexcept
        {
            return & *qbase;
        }


        const qmatrix * qfamily:: operator->() const noexcept
        {
            return & *qbase;
        }

        std::ostream & operator<<(std::ostream &os, const qfamily &self)
        {
            os << *(self.qbase) << "@" << *(self.basis) << "+" << *(self.ready);
            return os;
        }


        qfamily:: qfamily(const qfamily &F) :
        object(),
        qmetrics(F),
        next(0),
        prev(0),
        qbase( F.qbase ),
        basis( F.basis ),
        ready( F.ready )
        {
        }

       



    }

}

