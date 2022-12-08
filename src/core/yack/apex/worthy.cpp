#include "yack/apex/worthy.hpp"
#include "yack/apex/kernel.hpp"
#include "yack/type/utils.hpp"
#include "yack/system/imported.hpp"


namespace yack
{

    static inline
    size_t checked_dims(const size_t dims)
    {
        if(dims<=0) throw imported::exception("worthy::qmetrics","null dimension");
        return dims;
    }

    worthy:: qmetrics:: qmetrics(const size_t dims) :
    dimension( checked_dims(dims) )
    {

    }

    worthy:: qmetrics::qmetrics(const qmetrics &_) throw() :
    dimension(_.dimension)
    {
    }



    worthy:: qmetrics:: ~qmetrics() throw()
    {
        coerce(dimension) = 0;
    }
}

namespace yack
{
    worthy:: qarray:: ~qarray() throw() {}
    
    void worthy:: qarray:: setup()
    {
#if 1
        apk::simplify( coerce(coef) );
        for(size_t i=coef.size();i>0;--i) {
            assert(1==coef[i].den);
            coerce(nrm2) += apn::squared(coef[i].num.n);
        }
#endif
    }
    
    worthy:: qarray:: qarray(const qarray &_) :
    qmetrics(_),
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

    static inline bool are_same_simplified(const readable<apq> &l,
                                           const readable<apq> &r) throw()
    {
        assert(l.size()==r.size());
        const size_t n = l.size();
        for(size_t i=1;i<=n;++i)
        {
            assert(1==l[i].den);
            assert(1==r[i].den);
            if( l[i].num != r[i].num ) return false;
        }
        return true;
    }

    static inline bool are_diff_simplified(const readable<apq> &l,
                                           const readable<apq> &r) throw()
    {
        assert(l.size()==r.size());
        const size_t n = l.size();
        for(size_t i=1;i<=n;++i)
        {
            assert(1==l[i].den);
            assert(1==r[i].den);
            if( l[i].num != r[i].num ) return true;
        }
        return false;
    }


    bool operator==(const worthy::qarray &lhs, const worthy::qarray &rhs) throw()
    {
        if(are_same_simplified(lhs.coef,rhs.coef))
        {
            assert(lhs.nrm2==rhs.nrm2);
            return true;
        }
        else
            return false;
    }



    bool operator!=(const worthy::qarray &lhs, const  worthy::qarray &rhs) throw()
    {
        if(are_diff_simplified(lhs.coef,rhs.coef))
        {
            return true;
        }
        else
        {
            assert(lhs.nrm2==rhs.nrm2);
            return false;
        }
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



namespace yack
{
    worthy:: qcache::  qcache(const size_t dims) : qmetrics(dims), used() {}
    worthy:: qcache:: ~qcache() throw()
    {
    }

    const pool_of<worthy::qarray> & worthy:: qcache:: operator*()   const throw() { return used; }
    const pool_of<worthy::qarray> * worthy:: qcache:: operator->()  const throw() { return &used; }


    void  worthy:: qcache:: keep(qarray *q) throw()
    {
        assert(NULL!=q);
        assert(dimension==q->dimension);
        assert(dimension==q->coef.size());
        used.store(q);
        writable<apq> &cf = coerce(q->coef);
        for(size_t i=dimension;i>0;--i) cf[i].ldz();
        coerce(q->nrm2).ldz();
    }

    void worthy:: qcache:: release() throw()
    {
        used.release();
    }


    worthy::qarray * worthy:: qcache:: copy(const qarray &other)
    {
        assert(other.dimension == dimension);
        if(used.size<=0)
        {
            return new qarray(other);
        }
        else
        {
            qarray        &qa = *used.head; assert(dimension==qa.dimension);
            writable<apq> &cf = coerce(qa.coef);
            for(size_t i=dimension;i>0;--i) cf[i] = other.coef[i];
            coerce(qa.nrm2) = other.nrm2;
            return used.query();
        }
    }

}


namespace yack
{
    worthy:: qarrays:: qarrays(const qshared &cache) throw() :
    base(),
    repo(cache)
    {
    }

    worthy:: qarrays:: ~qarrays() throw()
    {
        quit();
    }

    void  worthy:: qarrays:: pop_back() throw()
    {
        assert(base.size);
        assert(base.tail);
        repo->keep( base.pop_back() );
    }


    void worthy:: qarrays:: release() throw()
    {
        quit();
    }


    void worthy:: qarrays:: quit() throw()
    {
        while(base.size) repo->keep( base.pop_back() );
    }

    worthy:: qarrays:: qarrays(const qarrays &other) :
    base(),
    repo(other.repo)
    {
        for(const qarray *node=other.base.head;node;node=node->next)
            base.push_back( repo->copy(*node) );
    }

    const list_of<worthy::qarray> & worthy:: qarrays:: operator*() const throw() { return base; }

    const list_of<worthy::qarray> * worthy:: qarrays:: operator->() const throw() { return &base; }


}


#include "yack/ptr/auto.hpp"
#include "yack/sort/indexing.hpp"

namespace yack
{

    static worthy::maturity initial_situation(const size_t dims)
    {
        switch(dims)
        {
            case 0: throw  imported::exception("Gram-Schmidt algorithm","null dimension");
            case 1: return worthy::fully_grown;
            case 2: return worthy::almost_done;
            default:
                break;
        }
        return worthy::in_progress;
    }

    worthy::maturity worthy::maturity_of(const size_t dims, const size_t size) throw()
    {
        assert(dims>0);
        if(size>=dims)
        {
            assert(dims==size);
            return worthy::fully_grown;
        }
        else
        {
            const size_t mark = dims-1;
            if(size<mark)
            {
                return worthy::in_progress;
            }
            else
            {
                assert(dims-1==size);
                return worthy::almost_done;
            }
        }
    }




    worthy:: qfamily:: qfamily(const qshared &cache)  :
    qmetrics( *cache ),
    situation( initial_situation(dimension) ),
    u_k(dimension),
    v_k(dimension),
    U(cache),
    Q(dimension),
    I(dimension)
    {
    }
    
    worthy:: qfamily:: ~qfamily() throw()
    {
    
    }
    
    worthy:: qfamily:: qfamily(const qfamily &_) :
    qmetrics(_),
    situation(_.situation),
    u_k(dimension),
    v_k(dimension),
    U(_.U),
    Q(dimension),
    I(dimension)
    {
        assert(U->size==_.U->size);

        // rebuild Q from new arrays
        for(qarray *q=U->head;q;q=q->next)
            Q << &coerce(q->coef);
        assert(Q.size()==U->size);

        // copy series
        const size_t n=Q.size();
        for(size_t i=1;i<=n;++i)
            I << _.I[i];

        assert(I.size()==Q.size());
        assert(I == _.I);
    }

    void worthy:: qfamily:: reset() throw()
    {
        U.release();
        Q.free();
        I.free();
        coerce(situation) = initial_situation(dimension);
    }

    const list_of<worthy::qarray> * worthy:: qfamily:: operator->()  const throw()
    {
        return & *U;
    }


    const list_of<worthy::qarray> & worthy:: qfamily:: operator*()  const throw()
    {
        return *U;
    }

    std::ostream & operator<<(std::ostream &os, const worthy::qfamily &self)
    {
        os << *(self.U) << " <" << self.I << ">";
        return os;
    }


    static inline
    int compare_coeffs(const readable<apq> *lhs, const readable<apq> *rhs) throw()
    {
        assert(lhs);
        assert(rhs);

        const readable<apq> &l = *lhs;
        const readable<apq> &r = *rhs; assert(l.size()==r.size());
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
        for(const qarray *node=U->head;node;node=node->next)
        {
            const readable<apq> &u_j = node->coef; assert(node->nrm2>0);
            const apq            cof = node->weight(v_k);
            apq                  sum = 0;
            for(size_t i=dimension;i>0;--i)
            {
                sum += (u_k[i] -= cof * u_j[i]).num.n;
            }
            assert(0==apq_dot(u_k,u_j));
            if(sum<=0) return false;       // early return
        }
        
        //----------------------------------------------------------------------
        //
        // create a new simplified array
        //
        //----------------------------------------------------------------------
        assert(U->size<dimension);
        const qarray &qa = U.push_back(u_k);
        if( 0 != qa.nrm2 )
        {

            apk::set_univocal( coerce(qa.coef) );
            // register new coef and dummy index
            Q << &coerce(qa.coef); assert(Q.size()==U->size);
            I << 0;                assert(I.size()==U->size);

            // upgrading indices
            indexing::make(I, compare_coeffs, Q);

            // upgrading situation
            coerce(situation) = maturity_of(dimension,U->size);

            return true;
        }
        else
        {
            U.pop_back();
            return false;
        }

#if 0
        auto_ptr<qarray> pq( new qarray(u_k) );
        if( 0 != pq->nrm2 )
        {
            apk::set_univocal( coerce(pq->coef) );         // univocal setup
            //Q << &coerce(U->push_back( pq.yield() )->coef); // no-throw update
            I << 0;                                        // no-throw update
            assert(Q.size()==U->size);
            assert(I.size()==U->size);

            // upgrading indices
            indexing::make(I, compare_coeffs, Q);

            // upgrading situation
            coerce(situation) = maturity_of(dimension,U->size);

            return true;
        }
        else
        {
            return false;
        }
#endif
        
    }


    bool worthy:: qfamily:: eq(const qfamily &lhs, const qfamily &rhs) throw()
    {
        const list_of<qarray> &L = *lhs.U;
        const list_of<qarray> &R = *rhs.U;
        if(L.size==R.size)
        {
            const size_t                   nn = L.size;
            const readable<size_t>        &li = lhs.I; assert(li.size()==nn);
            const readable<size_t>        &ri = rhs.I; assert(ri.size()==nn);
            const readable<coefficients*> &lq = lhs.Q; assert(lq.size()==nn);
            const readable<coefficients*> &rq = rhs.Q; assert(rq.size()==nn);

            for(size_t k=1;k<=nn;++k)
            {
                const readable<apq> &l = *lq[li[k]]; // lhs coefficients #rank=k
                const readable<apq> &r = *rq[ri[k]]; // rhs coefficients #rank=k
                if(are_diff_simplified(l,r)) return false;
            }

            // all the same
            return true;
        }
        else
        {
            // different sizes
            return false;
        }
    }


    bool operator==(const worthy::qfamily &lhs, const worthy::qfamily &rhs) throw()
    {
        return worthy::qfamily::eq(lhs,rhs);
    }

    const readable<apq> & worthy:: qfamily:: project_()
    {
        for(const qarray *node=U->head;node;node=node->next)
        {
            const readable<apq> &u = node->coef;
            const apq            f = apq_dot(v_k,u)/node->nrm2;
            for(size_t i=dimension;i>0;--i)
            {
                u_k[i] += f * u[i];
            }
        }
        apk::simplify(u_k);
        return u_k;
    }

    
}


