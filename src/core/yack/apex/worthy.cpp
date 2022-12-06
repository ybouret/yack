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

    std::ostream & operator<<(std::ostream &os, const worthy::qarray &self)
    {
        os << '|' << self.coef << '|' << '=' << self.nrm2;
        return os;
    }

    bool operator==(const worthy::qarray &lhs, const worthy::qarray &rhs) throw()
    {
        const readable<apq> &l = lhs.coef;
        const readable<apq> &r = rhs.coef;  assert(l.size()==r.size());
        const size_t         n = l.size();
        for(size_t i=1;i<=n;++i)
        {
            assert(1==l[i].den);
            assert(1==r[i].den);

            if( l[i].num != r[i].num ) return false;
        }

        assert(lhs.nrm2==rhs.nrm2);
        return true;
    }

    bool operator!=(const  worthy::qarray &lhs, const  worthy::qarray &rhs) throw()
    {
        const readable<apq> &l = lhs.coef;
        const readable<apq> &r = rhs.coef;  assert(l.size()==r.size());
        const size_t         n = l.size();
        for(size_t i=1;i<=n;++i)
        {
            assert(1==l[i].den);
            assert(1==r[i].den);

            if( l[i].num != r[i].num )
            {
                return true;
            }
        }

        assert(lhs.nrm2==rhs.nrm2);
        return false;
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
#include "yack/sort/indexing.hpp"

namespace yack
{
    
    worthy:: qfamily:: qfamily(const size_t dims)  :
    dimension(dims),
    u_k(dimension),
    v_k(dimension),
    U(),
    Q(dimension),
    I(dimension)
    {
    }
    
    worthy:: qfamily:: ~qfamily() throw()
    {
    
    }
    
    worthy:: qfamily:: qfamily(const qfamily &_) :
    dimension(_.dimension),
    u_k(dimension),
    v_k(dimension),
    U(_.U),
    Q(dimension),
    I(dimension)
    {
        assert(U.size==_.U.size);
        for(qarray *q=U.head;q;q=q->next)
        {
            Q << q;
        }
        assert(Q.size()==U.size);

        const size_t n=Q.size();
        for(size_t i=1;i<=n;++i)
        {
            I << _.I[i];
        }
        
        assert(I.size()==Q.size());
        assert(I == _.I);
    }

    void worthy:: qfamily:: reset() throw()
    {
        U.release();
        Q.free();
        I.free();
    }

    bool worthy::qfamily:: fully_grown() const throw()
    {
        assert(Q.size()==U.size);
        assert(I.size()==U.size);
        return U.size>=dimension;
    }

    std::ostream & operator<<(std::ostream &os, const worthy::qfamily &self)
    {
        os << self.U << " #" << self.I;
        return os;
    }


    static inline
    int compare_qarrays(const worthy::qarray *lhs, const worthy::qarray *rhs) throw()
    {
        assert(lhs);
        assert(rhs);

        const readable<apq> &l = lhs->coef;
        const readable<apq> &r = rhs->coef; assert(l.size()==r.size());
        const size_t         n = l.size();

        for(size_t i=1;i<=n;++i)
        {
            switch( __sign::of(l[i].num,r[i].num) )
            {
                case negative: assert(l[i].num<r[i].num);
                    return -1;

                case __zero__:
                    continue;

                case positive: assert(r[i].num<l[i].num);
                    return 1;
            }
        }

        // shouldn't happen
        return 0;
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
            Q << U.push_back( pq.yield() ); // no-throw
            I << 0;                         // no-throw
            assert(Q.size()==U.size);
            assert(I.size()==U.size);

            indexing::make(I, compare_qarrays, Q);

            return true;
        }
        else
        {
            return false;
        }
        
    }

    
}


