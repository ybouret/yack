
#include "yack/woven/qfamily.hpp"
#include "yack/system/imported.hpp"

namespace yack
{
    namespace woven
    {

        const char qfamily:: clid[] = "qfamily";


        qfamily:: qfamily(const size_t         dims,
                          const indices::fund &fund) :
        spot_object(),
        metrics(dims),
        qvectors(),
        indx(fund),
        base(fund),
        deps(fund),
        next(0),
        prev(0),
        qarr(dimensions),
        qtmp(dimensions)
        {

        }

        qfamily:: ~qfamily() noexcept
        {
        }

        qfamily:: qfamily(const qfamily &F) :
        spot_object(),
        metrics(F),
        qvectors(F),
        indx(F.indx),
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

        bool qfamily:: contains()
        {
            assert(size>0);

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


        void qfamily:: initialize(const size_t ndof,
                                  const size_t roll)
        {
            assert(ndof>=1);
            assert(roll>=1);
            assert(roll<=ndof);

            release();
            
            indx.clear();
            base.free();
            deps.free();

            for(size_t j=1;j<=ndof;++j)  indx << j;
            for(size_t k=roll-1;k>0;--k) indx.roll_down();

        }

        std::ostream & operator<<(std::ostream &os, const qfamily &f)
        {
            os << "  <base=" << *(f.base) << " | deps=" << *(f.deps) << " | indx=" << (f.indx) << ">" << std::endl;
            for(const qvector *v=f.head;v;v=v->next) {
                os << "       |_" << *v;
                if(v->next) os << std::endl;
            }
            return os;
        }

        bool qfamily:: used(const size_t idx) const noexcept
        {
            return base.contains(idx) || deps.contains(idx);
        }


    }
    
}

