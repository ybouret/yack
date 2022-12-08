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


#include "yack/ptr/auto.hpp"
#include "yack/sort/indexing.hpp"
#include "yack/system/imported.hpp"

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




    worthy:: qfamily:: qfamily(const size_t dims)  :
    dimension( dims ),
    situation( initial_situation(dimension) ),
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
    situation(_.situation),
    u_k(dimension),
    v_k(dimension),
    U(_.U),
    Q(dimension),
    I(dimension)
    {
        assert(U.size==_.U.size);

        // rebuild Q from new arrays
        for(qarray *q=U.head;q;q=q->next)
            Q << &coerce(q->coef);
        assert(Q.size()==U.size);

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


    std::ostream & operator<<(std::ostream &os, const worthy::qfamily &self)
    {
        os << self.U << " <" << self.I << ">";
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
            if(sum<=0) return false;       // early return
        }
        
        //----------------------------------------------------------------------
        //
        // create a new simplified array
        //
        //----------------------------------------------------------------------
        auto_ptr<qarray> pq( new qarray(u_k) );
        if( 0 != pq->nrm2 )
        {
            apk::set_univocal( coerce(pq->coef) );         // univocal setup
            Q << &coerce(U.push_back( pq.yield() )->coef); // no-throw update
            I << 0;                                        // no-throw update
            assert(Q.size()==U.size);
            assert(I.size()==U.size);

            // upgrading indices
            indexing::make(I, compare_coeffs, Q);

            // upgrading situation
            coerce(situation) = maturity_of(dimension,U.size);

            return true;
        }
        else
        {
            return false;
        }
        
    }


    bool worthy:: qfamily:: eq(const qfamily &lhs, const qfamily &rhs) throw()
    {
        const list_of<qarray> &L = lhs.U;
        const list_of<qarray> &R = rhs.U;
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

    const readable<apq> & worthy:: qfamily:: project()
    {
        for(const qarray *node=U.head;node;node=node->next)
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


