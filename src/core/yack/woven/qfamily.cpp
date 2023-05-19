
#include "yack/woven/qfamily.hpp"

namespace yack
{
    namespace woven
    {
        qfamily:: qfamily(const size_t dims) :
        metrics(dims),
        qvectors(),
        qarr(dimensions),
        qtmp(dimensions),
        next(0), prev(0)
        {
        }

        qfamily:: ~qfamily() noexcept
        {
        }

        qfamily:: qfamily(const qfamily &F) :
        metrics(F),
        qvectors(F),
        qarr(dimensions),
        qtmp(dimensions),
        next(0),
        prev(0)
        {
        }


        bool qfamily:: accepts()
        {
            assert(size>0);
            if(size>=dimensions)
            {
                return false;
            }
            else
            {
                for(const qvector *qvec=head;qvec;qvec=qvec->next)
                {
                    if(!qvec->ortho(qtmp,qarr)) return false; // no more orthogonal component
                    for(size_t i=dimensions;i>0;--i)
                    {
                        qarr[i].xch(qtmp[i]);
                    }
                }
                return true;
            }
        }

        bool qfamily:: contains()
        {
            assert(size>0);
            if(size>=dimensions)
            {
                return true;
            }
            else
            {
                for(const qvector *qvec=head;qvec;qvec=qvec->next)
                {
                    if(!qvec->ortho(qtmp,qarr)) return true; // no more orthogonal component
                    for(size_t i=dimensions;i>0;--i)
                    {
                        qarr[i].xch(qtmp[i]);
                    }
                }
                return false; // projected vector is not zero
            }
        }

        void qfamily:: grow()
        {
            assert( q_array::check_not_null(qarr) );
            push_back( new qvector(qarr) );
        }

    }
}
