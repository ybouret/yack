
#include "yack/chem/nexus.hpp"
#include "yack/counting/comb.hpp"
#include "yack/data/list/sort.hpp"
#include "yack/apex/kernel.hpp"
#include "yack/math/iota.hpp"
#include "yack/system/imported.hpp"
#include "yack/math/algebra/ortho-family.hpp"
#include "yack/sequence/cxx-array.hpp"
#include "yack/sequence/cxx-series.hpp"
#include "yack/sequence/roll.hpp"

#include <iomanip>

namespace yack
{
    using namespace math;
    
    namespace chemical
    {

        static inline apz q2z(const apq &q)
        {
            assert(1==q.den);
            return q.num;
        }

        static inline
        void buildU(matrix<apq>            &U,
                    const matrix<apq>      &Q,
                    const size_t            ir,
                    const readable<size_t> &isub)
        {
            iota::load(U[1],Q[ir]);
            for(size_t i=U.rows;i>1;--i)
            {
                iota::load(U[i],Q[isub[i-1]]);
            }
        }

        static inline
        bool is_candidate(const readable<apq> &arr) throw()
        {
            size_t count = 0;
            for(size_t i=arr.size();i>0;--i)
            {
                assert(1==arr[i].den);
                switch(arr[i].num.s)
                {
                    case __zero__:
                        continue;

                    case positive:
                        ++count;
                        continue;


                    case negative:
                        return false;
                }
            }
            return count>=2;
        }

        namespace
        {
            //! unsigned weights
            typedef cxx_array<unsigned> iweight;

            //! weights in a node
            class wnode : public object, public iweight
            {
            public:
                // C++
                inline explicit wnode(const readable<apq> &q) :
                object(), iweight(q.size()), next(0)
                {
                    writable<unsigned> &self = *this;
                    for(size_t i=self.size();i>0;--i)
                    {
                        assert(1==q[i].den);
                        assert(q[i].num.s != negative);
                        self[i] = q[i].num.n.cast_to<unsigned>();
                    }
                }

                inline virtual ~wnode() throw() {}

                // member
                wnode *next;

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(wnode);
            };

            //! base class for group of weights
            typedef cxx_pool_of<wnode> wgroup;

            //! repository
            class wrepo : public wgroup
            {
            public:
                inline explicit wrepo() throw() : wgroup() {}
                inline virtual ~wrepo() throw() {}

                inline void ensure(const readable<apq> &q)
                {
                    assert(is_candidate(q));
                    const size_t    len = q.size() * sizeof(unsigned);
                    auto_ptr<wnode> lhs = new wnode(q);
                    const void     *l   = (*lhs)();
                    for(const wnode *rhs=head;rhs;rhs=rhs->next)
                    {
                        const void *r = (*rhs)();
                        if(0==memcmp(l,r,len))
                        {
                            std::cerr << "already" << std::endl;
                            return;
                        }
                    }
                    std::cerr << "new" << std::endl;
                    store( lhs.yield() );
                }

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(wrepo);
            };

        }

        void nexus:: conserved_set(const xmlog &xml)
        {
            static const char * const fn = "conserved_set";
            YACK_XMLSUB(xml,fn);

            const size_t n = regular.size;
            if(n<=0)
            {
                YACK_XMLOG(xml, "-- no regular equilibrium");
                return;
            }

            wrepo W;
            {
                //--------------------------------------------------------------
                //
                // selecting conserved species within regular equilibria
                //
                //--------------------------------------------------------------
                YACK_XMLOG(xml, "-- selecting conserved species");
                const matrix<apq> P(n,M);
                {
                    size_t             ii=1;
                    for(const eq_node *en=regular.head;en;en=en->next,++ii)
                    {
                        const equilibrium   &eq = **en;
                        const size_t         ei = *eq;
                        const readable<int> &nu = Nu[ei];
                        writable<apq>       &Pi = coerce(P[ii]);
                        for(const cnode *cn=eq.head();cn;cn=cn->next)
                        {
                            const species &s = ****cn;
                            const size_t   j = *s;
                            if(conserved==crit[j]) Pi[j] = nu[j];
                        }
                    }
                }
                if(verbose) std::cerr << "P=" << P << std::endl;


                //--------------------------------------------------------------
                //
                // building orthogonal family
                //
                //--------------------------------------------------------------
                const matrix<apq> Q(M,M);
                if( !ortho_family::build(coerce(Q),P) )
                {
                    YACK_XMLOG(xml, "-- singular conserved set");
                    return;
                }
                const size_t  rank = apk::rank_of(Q);

                if(verbose)
                {
                    std::cerr << "Q=" << Q << std::endl;
                    *xml << "-- rank(Q) = " << rank << std::endl;
                }

                //--------------------------------------------------------------
                //
                // selecting non proportional rows of Q
                //
                //--------------------------------------------------------------
                cxx_series<size_t> irow(M);
                cxx_series<size_t> isub(M-1);

                {
                    matrix<apz> ZQ(q2z,Q);
                    for(size_t i=1;i<=M;++i)
                    {
                        bool ok = true;
                        for(size_t j=irow.size();j>0;--j)
                        {
                            const size_t jr = irow[j];
                            if( apk::are_prop(ZQ[jr],ZQ[i],NULL))
                            {
                                ok = false;
                                break;
                            }
                        }
                        if(ok)
                            irow << i;
                    }
                }

                YACK_XMLOG(xml,"irow=" << irow);

                const size_t m = irow.size(); assert(m>=rank);
                matrix<apq>  U(rank,M);

                //--------------------------------------------------------------
                //
                // building all
                //
                //--------------------------------------------------------------
                for(size_t i=1;i<=m;++i)
                {
                    const size_t ir = irow[i];
                    isub.free();
                    for(size_t ii=1;ii<i;++ii)    isub << irow[ii];
                    for(size_t ii=i+1;ii<=m;++ii) isub << irow[ii];
                    for(size_t rotation=0;rotation<m;++rotation)
                    {
                        buildU(U,Q,ir,isub);
                        if(  apk::gs_ortho(U) )
                        {
                            for(size_t ii=2;ii<=rank;++ii)
                            {
                                const readable<apq> &u = U[ii];
                                if(is_candidate(u)) {
                                    std::cerr << "-> " << u << std::endl;
                                    //auto_ptr<wnode> w = new wnode(u);
                                    W.ensure(u);
                                }
                            }
                        }
                        rolling::down(isub);
                    }

                }
            }



        }
    
                                
        
    }
    
}

