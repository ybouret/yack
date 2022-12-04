#include "yack/apex/worthy.hpp"
#include "yack/apex/kernel.hpp"
#include "yack/type/utils.hpp"

namespace yack
{
    worthy:: qarray:: ~qarray() throw() {}
    
    void worthy:: qarray:: setup()
    {
        apk::simplify( coerce(coef) );
        for(size_t i=coef.size();i>0;--i) {
            assert(1==coef[i].den);
            coerce(nrm2) += apn::squared(coef[i].num.n);
        }
    }
    
    worthy:: qarray:: qarray(const qarray &_) :
    object(),
    next(0),
    prev(0),
    coef(_.coef),
    nrm2(_.nrm2)
    {
    }
    
    static inline apq apq_dot(const readable<apq> &lhs, const readable<apq> &rhs)
    {
        assert( lhs.size() == rhs.size() );
        apq res = lhs[1] * rhs[1];
        for(size_t i=lhs.size();i>1;--i)
        {
            res += lhs[i] * rhs[i];
        }
        return res;
    }
    apq worthy:: qarray:: weight(const readable<apq> &v) const
    {
        assert( v.size() == coef.size() );
        return apq_dot(v,coef)/nrm2;
    }

    
}


#include "yack/ptr/auto.hpp"
#include "yack/system/imported.hpp"

namespace yack
{
    
    worthy:: qfamily:: qfamily(const size_t dims)  :
    dimension(dims),
    u_k(dimension),
    v_k(dimension),
    U()
    {
    }
    
    worthy:: qfamily:: ~qfamily() throw()
    {
    
    }
    
    worthy:: qfamily:: qfamily(const qfamily &_) :
    dimension(_.dimension),
    u_k(dimension),
    v_k(dimension),
    U(_.U) 
    {
    }

    void worthy:: qfamily:: release() throw()
    {
        U.release();
    }
    
    bool worthy::qfamily:: try_grow()
    {
        //----------------------------------------------------------------------
        //
        // u_k = v_k is loaded : apply Gram-Schmidt
        //
        //----------------------------------------------------------------------
        for(const qarray *node=U.head;node;node=node->next)
        {
            const readable<apq> &u_j = node->coef; assert(node->nrm2>0);
            const apq            cof = node->weight(v_k);
            apq                  sum = 0;
            for(size_t i=dimension;i>0;--i)
            {
                sum += (u_k[i] -= cof * u_j[i]).num.n;
            }
            assert(0==apq_dot(u_k,u_j));
            if(sum<=0) return false;
        }
        
        //----------------------------------------------------------------------
        //
        // create a new simplified array
        //
        //----------------------------------------------------------------------
        auto_ptr<qarray> pq( new qarray(u_k) );
        if( 0 != pq->nrm2 )
        {
            apk::set_univocal( coerce(pq->coef) );
            U.push_back( pq.yield() );
            return true;
        }
        else
        {
            return false;
        }
        
    }

    
}


