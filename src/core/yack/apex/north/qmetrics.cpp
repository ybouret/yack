
#include "yack/apex/north/qmetrics.hpp"
#include "yack/system/imported.hpp"
#include "yack/apex/kernel.hpp"
#include "yack/type/utils.hpp"

namespace yack
{

    namespace north
    {
        static const char here[] = "north";
        static const char mesg[] = "dimension<=1!";

        size_t qmetrics:: checked_dimension(const size_t d)
        {
            if(d<=1) throw imported::exception(here,mesg);
            return d;
        }

        
        
        maturity qmetrics:: updated_situation(const size_t dims, const size_t size) throw()
        {
            assert(dims>1);
            if(size>=dims)
            {
                assert(dims==size);
                return fully_grown;
            }
            else
            {
                const size_t mark = dims-1;
                if(size<mark)
                {
                    return  in_progress;
                }
                else
                {
                    assert(dims-1==size);
                    return  almost_done;
                }
            }
        }

        


        static inline
        void zsimplify(apn &norm2, writable<apz> &z)
        {
            assert(z.size()>1);

            norm2.ldz();
            const size_t n = z.size();

            //----------------------------------------------------------------------
            // find first positive value
            //----------------------------------------------------------------------
            apn          g = 1;
            size_t       j = 1;
            for(;j<=n;++j)
            {
                const apz &Z = z[j];
                if(__zero__!=Z.s) {
                    g = Z.n;
                    break;
                }
            }

            //----------------------------------------------------------------------
            // compute with other positive values
            //----------------------------------------------------------------------
            for(++j;j<=n;++j)
            {
                const apz &Z = z[j];
                if(__zero__!=Z.s) {
                    g = apn::gcd(Z.n,g);
                }
            }

            //----------------------------------------------------------------------
            // simplify
            //----------------------------------------------------------------------
            for(size_t i=n;i>0;--i)
            {
                norm2 += apn::squared( (z[i] /= g).n );
            }

        }


        void qmetrics:: prepare_vector(writable<apz> &target,
                                       writable<apq> &source,
                                       apn           &normSq)
        {
            assert(source.size()>=2);
            assert(source.size()==target.size());

            static const size_t one = 1;
            const size_t        dim = source.size();
            const apn           rho = apk::lcm(source,one,dim);

            for(size_t i=dim;i>0;--i)
            {
                const apq & q = source[i];
                target[i] = (q.num*rho/q.den);
            }
            zsimplify(normSq,target);
            apk::univocal(target);
        }

        
        qmetrics:: ~qmetrics() throw()
        {
            coerce(dimension) = 0;
        }

        qmetrics:: qmetrics(const size_t dims) :
        ios::serializable(),
        dimension( checked_dimension(dims) )
        {
        }


        qmetrics:: qmetrics(const qmetrics &other) throw() :
        ios::serializable(),
        dimension(other.dimension)
        {
            assert(dimension>0);
        }

    }

}
