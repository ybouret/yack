
#include "yack/woven/qfamily.hpp"
#include "yack/system/imported.hpp"

namespace yack
{
    namespace woven
    {

        const char qfamily:: clid[] = "qfamily";

        qfamily:: qfamily(const size_t    dims,
                          const size_t    rmax,
                          const indxFund &isrc) :
        metrics(dims),
        qvectors(),
        code(isrc),
        base(isrc),
        deps(isrc),
        next(0),
        prev(0),
        qarr(dimensions),
        qtmp(dimensions)
        {
            for(size_t i=1;i<=rmax;++i) code << i;
        }

        qfamily:: ~qfamily() noexcept
        {
        }

        qfamily:: qfamily(const qfamily &F) :
        metrics(F),
        qvectors(F),
        code(F.code),
        base(F.base),
        deps(F.deps),
        next(0),
        prev(0),
        qarr(dimensions),
        qtmp(dimensions)
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
                    if(!qvec->make_ortho(qtmp,qarr)) return false; // no more orthogonal component
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
                    if(!qvec->make_ortho(qtmp,qarr)) return true; // no more orthogonal component
                    for(size_t i=dimensions;i>0;--i)
                    {
                        qarr[i].xch(qtmp[i]);
                    }
                }
                return false; // projected vector is not zero
            }
        }

        void qfamily:: grow(const size_t idx)
        {
            assert( q_array::check_not_null(qarr) );
            assert( !base.contains(idx) );
            assert( !deps.contains(idx) );
            
            push_back( new qvector(qarr) );
            try {  base += idx; }
            catch(...)
            {
                delete pop_back();
                throw;
            }
        }

        void qfamily:: drop(const size_t idx)
        {
            assert( !base.contains(idx) );
            assert( !deps.contains(idx) );
            deps += idx;
        }

    }
}
