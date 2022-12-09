#include "yack/apex/north/north.hpp"
#include "yack/system/imported.hpp"
#include "yack/memory/allocator/global.hpp"
#include "yack/apex/kernel.hpp"

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

        template <>
        apq qvector<apq>:: weight(const readable<apq> &v) const
        {
            assert(v.size()==dimension);
            apq sum = 0;
            const readable<apq> &self = *this;
            for(size_t i=dimension;i>0;--i)
            {
                sum += v[i] * self[i];
            }
            return sum/norm2;
        }

        template <>
        apq qvector<int64_t>:: weight(const readable<apq> &v) const
        {
            assert(v.size()==dimension);
            apq sum = 0;
            const readable<int64_t> &self = *this;
            for(size_t i=dimension;i>0;--i)
            {
                const apq rhs(self[i]);
                sum += v[i] * rhs;
            }
            return sum/norm2;
        }

        bool constellation:: are_orthogonal(const readable<apq> &lhs,
                                            const readable<apq> &rhs)
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
        
        

    }

}
