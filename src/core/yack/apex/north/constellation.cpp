
#include "yack/apex/north/constellation.hpp"
#include "yack/system/imported.hpp"
#include "yack/apex/kernel.hpp"
#include "yack/type/utils.hpp"

namespace yack
{

    namespace north
    {
        static const char here[] = "north";
        static const char mesg[] = "null dimension!";

        size_t constellation:: checked_dimension(const size_t d)
        {
            if(d<=0) throw imported::exception(here,mesg);
            return d;
        }

        maturity constellation:: initial_situation(const size_t dims)
        {
            switch(dims)
            {
                case 0: throw  imported::exception(here,mesg);
                case 1: return fully_grown;
                case 2: return almost_done;
                default:
                    break;
            }
            return in_progress;
        }

        maturity constellation:: updated_situation(const size_t dims, const size_t size) throw()
        {
            assert(dims>0);
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

        bool constellation:: are_orthogonal(const readable<apq> &lhs,
                                            const readable<apq> &rhs)
        {
            assert( lhs.size() == rhs.size() );
            apq sum = 0;
            for(size_t i=lhs.size();i>0;--i) sum += lhs[i] * rhs[i];
            return 0 == sum;
        }

        bool constellation:: are_orthogonal(const readable<apq>     &lhs,
                                            const readable<int64_t> &rhs)
        {
            assert( lhs.size() == rhs.size() );
            apq sum = 0;
            for(size_t i=lhs.size();i>0;--i) sum += lhs[i] * rhs[i];
            return 0 == sum;
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

        bool constellation:: prepare_vector(writable<int64_t> &target,
                                            writable<apq>     &source,
                                            uint64_t          &normSq)
        {
            static const char here[] = "north.prepare_vector";

            assert(source.size()==target.size());
            normSq = 0;
            apk::simplify(source);
            for(size_t i=source.size();i>0;--i)
            {
                assert(1==source[i].den);
                const apz &num = source[i].num;
                target[i] = num.cast_to<int64_t>(here);
                normSq   += squared(target[i]);
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

    }

}
