
#include "yack/counting/part.hpp"
#include "yack/counting/c/part.h"
#include "yack/system/exception.hpp"
#include "yack/sequence/vector.hpp"
#include "yack/sequence/list.hpp"
#include <cerrno>

namespace yack
{
    partition:: ~partition() throw()
    {
    }
    
    static inline
    size_t check_part(const size_t n)
    {
        if(n<=0) throw libc::exception(EDOM,"partition(0)");
        return n;
    }
    
    partition:: partition(const size_t n) :
    schedule(sizeof(yack_part),check_part(n))
    {
        yack_part_init( static_cast<yack_part*>(addr), n);
        yack_part_boot( static_cast<yack_part*>(addr),data);
    }
    
    size_t partition:: size() const throw()
    {
        return static_cast<yack_part*>(addr)->k;
    }
    
    void partition:: boot() throw()
    {
        yack_part_boot( static_cast<yack_part*>(addr),data);
    }
    
    bool  partition::  next() throw()
    {
        return  yack_part_next( static_cast<yack_part*>(addr),data) != 0;
    }


    static inline void euler_sum(apn &sum, const list<apn> &l)
    {
        for(list<apn>::const_iterator it=l.begin();it!=l.end();++it)
        {
            sum += *it;
        }
    }

    static inline void euler_update(vector<apn> &p)
    {
        // initialize p to its new size
        apn  tmp = 0;
        p << tmp;
        const size_t n = p.size();


        list<apn>    add; // list of positive terms
        list<apn>    sub; // list of negative terms
        tmp = 1;
        for(size_t k=1;;++k)
        {
            const size_t k3  = k*3;
            const size_t km  = k*(k3-1)/2;
            const size_t kp  = k*(k3+1)/2;
            list<apn>   &dst = ( 0 == (k&1) ) ? sub : add;
            bool         brk = false;

            switch(__sign::of(km,n) )
            {
                case negative: assert(km<n);  dst.push_back(p[n-km]); break;
                case __zero__: assert(n==km); dst.push_back(tmp);     break;
                case positive: brk = true; break;
            }

            switch(__sign::of(kp,n) )
            {
                case negative: assert(kp<n);  dst.push_back(p[n-kp]); break;
                case __zero__: assert(n==kp); dst.push_back(tmp);     break;
                case positive:  break;
            }
            

            if(brk) break;
        }

        apn &lhs = p.back(); euler_sum(lhs,add);
        apn  rhs = 0;        euler_sum(rhs,sub);
        if(rhs>=lhs) throw libc::exception(EINVAL,"invalid Euler's algorithm for partiton of %lu",(unsigned long)n);
        lhs-=rhs;
    }

    static inline apn euler(const size_t n)
    {
        if(n<=2) return n;
        vector<apn> p(n,as_capacity);
        apn tmp = 1; p << tmp;
        tmp=2;       p << tmp;
        for(size_t i=3;i<=n;++i)
            euler_update(p);
        return p.back();
    }

    
    apn partition:: compute(const size_t n)
    {
        return euler(n);
    }

}
