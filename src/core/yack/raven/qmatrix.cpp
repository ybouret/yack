#include "yack/raven/qmatrix.hpp"
#include "yack/system/imported.hpp"

namespace yack
{
    namespace raven
    {

        qmatrix:: ~qmatrix() throw()
        {

        }

        static const char here[] = "raven::matrix";

        qmatrix:: qmatrix(const size_t sz,
                          const size_t rk) :
        collection(),
        object(),
        qmetrics(sz),
        readable<qvector>(),
        maximum_rank(rk),
        current_rank(0),
        active_state(maximum_rank>2 ? in_progress : almost_done),
        lib(),
        obj(),
        row(),
        vgs()
        {
            if(maximum_rank<2)         throw imported::exception(here,"maximum_rank<2");
            if(maximum_rank>dimension) throw imported::exception(here,"maximum_rank>dimension");
            initialize();
        }

        qmatrix:: qmatrix(const qmatrix &Q) :
        collection(),
        object(),
        qmetrics(Q),
        readable<qvector>(),
        maximum_rank(Q.maximum_rank),
        current_rank(Q.current_rank),
        active_state(Q.active_state),
        lib(),
        obj(),
        row(),
        vgs()
        {
            initialize();
            for(size_t i=current_rank;i>0;--i)
            {
                row[i].hard_copy(Q.row[i]);
            }
        }

        qmatrix * qmatrix:: clone() const { return new qmatrix(*this); }

        void qmatrix:: reset() throw()
        {
            coerce(current_rank) = 0;
            coerce(active_state) = (maximum_rank>2 ? in_progress : almost_done);
        }

        size_t    qmatrix:: size() const throw() { return current_rank; }

        const qvector & qmatrix:: operator[](const size_t ivec) const throw()
        {
            assert(ivec>=1); assert(ivec<=maximum_rank);
            return row[ivec];
        }

        static inline
        void display_qvector(const qvector &v, std::ostream &os)
        {
            assert(v.size()>=2);
            os << v[1];
            for(size_t i=2;i<=v.size();++i) os << ' ' << v[i];
        }

        std::ostream   & operator<<(std::ostream &os, const qmatrix &self)
        {
            os << '[';
            if(self.current_rank)
            {
                display_qvector(self.row[1],os);
                for(size_t i=2;i<=self.current_rank;++i)
                {
                    display_qvector(self.row[i],os << ';');
                }

            }
            os << ']';
            return os;
        }



        size_t  qmatrix:: allocated() const throw()
        {
            return lib.bytes;
        }

        void qmatrix:: keep_ortho(writable<apq>       &u_k,
                                  const readable<apz> &v_k)
        {
            for(size_t j=current_rank;j>0;--j)
                row[j].sub(u_k,v_k);
            
        }

        bool qmatrix:: is_nil_vec(const readable<apq> &u_k) const throw()
        {
            assert(dimension==u_k.size());
            for(size_t i=dimension;i>0;--i)
            {
                if( __zero__ != u_k[i].num.s ) return false;
            }
            return true;
        }

        bool qmatrix:: build_next(writable<apq>       &u_k,
                                  const readable<apz> &v_k)
        {
            keep_ortho(u_k,v_k);
            if(fully_grown==active_state)
            {
                if(!is_nil_vec(u_k)) throw imported::exception(here,"new vector while fully grown!");
                return false;
            }
            else
            {
                assert(current_rank<maximum_rank);
                const size_t working_rank = current_rank+1;
                qvector     &next_qvector = row[working_rank];
                if(!next_qvector.appointed(u_k))
                {
                    return false;
                }
                else
                {
                    coerce(current_rank) = working_rank;
                    switch(maximum_rank-current_rank)
                    {
                        case 0: coerce(active_state) = fully_grown; break;
                        case 1: coerce(active_state) = almost_done; break;
                        default: assert(in_progress==active_state); break;
                    }
                    return true;
                }
            }
        }

        bool qmatrix:: is_equivalent_to(const qmatrix &rhs)
        {
            if(current_rank != rhs.current_rank) return false;

            for(size_t i=current_rank;i>0;--i)
            {
                const readable<apz> &v = rhs[i];
                if(!includes(v)) return false;
            }
            
            return true;
        }

    }

}

#include "yack/apex/kernel.hpp"

namespace yack
{
    namespace raven
    {

        bool qmatrix:: try_polish(writable<apz>       &target,
                                  const readable<apq> &source) const
        {
            assert(target.size()==dimension);
            assert(source.size()==dimension);
            bool      gtz = false;
            {
                const apn rho = apk::lcm(source,size_t(1),dimension);
                for(size_t i=dimension;i>0;--i)
                {
                    const apq tmp = source[i] * rho; assert(tmp.den==1);
                    if( __zero__ != (target[i] = tmp.num).s ) gtz = true;
                }
            }
            
            if(gtz)
            {
                apn       dum;
                apk::definite(target,dum);
                return true;
            }
            else
                return false;
        }


    }

}

#include "yack/memory/embed.hpp"
#include "yack/memory/allocator/dyadic.hpp"
#include "yack/memory/allocator/global.hpp"

namespace yack
{
    namespace raven
    {

        void qmatrix:: initialize()
        {
            static memory::allocator &mgr = memory::dyadic::instance();
            //static memory::allocator &mgr = memory::global::instance();

            // qvectors
            qvector     *pqv = NULL;
            const size_t nqv = maximum_rank;

            // items
            apz         *pit = NULL;
            const size_t nit = dimension*maximum_rank + dimension;

            // G-S rational
            apq          *pgs = NULL;
            const size_t  ngs = dimension;
            memory::embed emb[] =
            {
                memory::embed(pqv,nqv),
                memory::embed(pit,nit),
                memory::embed(pgs,ngs)
            };

            lib.build(emb, sizeof(emb)/sizeof(emb[0]),mgr);

            { contractor<apz>     _obj(pit,nit);                         obj.swap_with(_obj); }
            { contractor<qvector> _row(pqv,nqv,pit+dimension,dimension); row.swap_with(_row); }
            { contractor<apq>     _vgs(pgs,ngs);                         vgs.swap_with(_vgs); }
        }

    }

}
