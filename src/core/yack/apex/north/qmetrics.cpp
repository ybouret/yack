
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

      

        bool constellation:: prepare_vector(writable<apq> &target,
                                            writable<apq> &source,
                                            apn           &normSq)
        {
            assert(source.size()==target.size());
            normSq.ldz();
            apk::simplify(source);
            for(size_t i=source.size();i>0;--i)
            {
                assert(1==source[i].den);
                const apz &num = source[i].num;
                target[i] = num;
                normSq   += apn::squared(num.n);
            }

            if(normSq<=0)
            {
                return false;
            }
            else
            {
                apk::univocal(target);
                return true;
            }
        }

        
        qmetrics:: ~qmetrics() throw()
        {
            coerce(dimension) = 0;
        }

        qmetrics:: qmetrics(const size_t dims) :
        dimension( checked_dimension(dims) )
        {
        }


        qmetrics:: qmetrics(const qmetrics &other) throw() :
        dimension(other.dimension)
        {
            assert(dimension>0);
        }

    }

}
