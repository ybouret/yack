
#include "yack/chem/reactor/cluster.hpp"
#include "yack/raven/qbranch.hpp"
#include "yack/apex/alga.hpp"
#include "yack/math/algebra/ortho-family.hpp"
#include "yack/sequence/cxx-series.hpp"
#include "yack/math/iota.hpp"
#include "yack/sequence/bunch.hpp"

namespace yack
{
    using namespace math;
    using namespace raven;

    namespace chemical
    {

        namespace {

            //! compress ortho family
            template <typename T>
            static inline
            void compressQ(matrix<T> &Q, const matrix<T> &Q0)
            {
                const size_t       n = Q0.rows;
                cxx_series<size_t> indx(n);

                for(size_t i=1;i<=n;++i)
                {
                    const readable<T>   &Qi = Q0[i];
                    bool                 ok = true;
                    for(size_t j=indx.size();j>0;--j)
                    {
                        if( alga::colinear(Qi,Q0[indx[j]]))
                        {
                            ok = false;
                            break;
                        }
                    }
                    if(!ok) continue;
                    indx << i;
                }
                const size_t q = indx.size();
                Q.make(q,Q0.cols);
                for(size_t j=1;j<=q;++j)
                    iota::load(Q[j],Q0[indx[j]]);
            }

            static inline unsigned sum_of(const readable<unsigned> &arr) throw()
            {
                unsigned res = 0;
                for(size_t i=arr.size();i>0;--i)
                {
                    res += arr[i];
                }
                return res;
            }
            
            static inline int compare_cb(const readable<unsigned> &lhs,
                                           const readable<unsigned> &rhs) throw()
            {
                
                assert(lhs.size()==rhs.size());
                const unsigned lsum = sum_of(lhs);
                const unsigned rsum = sum_of(rhs);
                
                if(lsum<rsum)
                {
                    return -1;
                }
                else
                {
                    if(rsum<lsum)
                    {
                        return 1;
                    }
                    else
                    {
                        return comparison::lexicographic(lhs,rhs);
                    }
                        
                }
                
            }
            
            //! collect valid combinations from RAVEn
            class collector : public bunch<unsigned>
            {
            public:
                const xmlog &xml;

                explicit collector(const size_t dim, const xmlog &_) :
                bunch<unsigned>(dim),
                xml(_)
                {
                }

                virtual ~collector() throw() { }

                bool validate(const readable<apz> &v) const throw()
                {
                    assert(v.size()==width);
                    size_t np = 0;
                    for(size_t i=width;i>0;--i)
                    {
                        switch(v[i].s)
                        {
                            case __zero__:
                                continue;
                            case positive:
                                ++np;
                                continue;
                            case negative:
                                return false;
                        }
                    }
                    return np>1;
                }

                void operator()(const qvector &v)
                {
                    const bool ok = validate(v) && insert( v.cast_to<unsigned>(work) );
                    if(ok)
                    {
                        YACK_XMLOG(xml, "[+] " << work);
                    }
                }


            private:
                YACK_DISABLE_COPY_AND_ASSIGN(collector);
            };

        }



        void cluster:: conservations(const xmlog &xml)
        {
            static const char * const fn = "cluster::conservations";
            YACK_XMLSUB(xml,fn);


            const size_t n = genus->bounded->size;

            if(n<=0) {
                YACK_XMLOG(xml, "no bounded equilibrium");
                return;
            }
            assert(breed->conserved->size);

            const size_t m   = (*alive)->size;
            collector    cb(m,xml);

            {
                //----------------------------------------------------------
                //
                // orthogonal space to local topology
                //
                //----------------------------------------------------------
                matrix<int>  Q;         // matrix to compute
                const size_t ker = m-n; // null space dimension = rank(Q)
                {
                    //----------------------------------------------------------
                    //
                    // build local topology of conserved species
                    //
                    //----------------------------------------------------------
                    matrix<int>  nu(n,m);
                    {
                        size_t i=1;
                        for(const eq_gnode *en=genus->bounded->head;en;en=en->next)
                        {
                            // get equilibrium
                            const equilibrium  &eq = (***en).host;
                            writable<int>      &ni = nu[i++];
                            size_t              ns = 0;

                            // look up for conserved species
                            for(const sp_gnode *sn=breed->conserved->head;sn;sn=sn->next)
                            {
                                const anode     &an = ***sn;
                                const species   &sp = an.host;
                                const component *cc = eq.query(sp);
                                if(cc) {
                                    const size_t J = *an;
                                    ni[J] = cc->nu;
                                    ++ns;
                                }
                            }

                            // sanity check
                            if(ns<=0) throw imported::exception(fn,"no conserved species in bounded <%s>!!", eq.name());
                        }
                    }
                    YACK_XMLOG(xml,"-- nu = " << nu);

                    //----------------------------------------------------------
                    //
                    // build compressed orthogonal space
                    //
                    //----------------------------------------------------------
                    {
                        matrix<int> Q0(m,m);
                        if(!ortho_family::build(Q0,nu,true))    throw imported::exception(fn,"singular sub-system");
                        compressQ(Q,Q0); if(ker!=alga::rank(Q)) throw imported::exception(fn,"singular compressed sub-system");
                    }
                }
                YACK_XMLOG(xml,"-- Q  = " << Q);


                {
                    YACK_XMLSUB(xml, "RAVEn");
                    qbranch  source;
                    source.batch(Q,ker,cb);
                }
            }

            if(cb->size)
            {
                YACK_XMLSUB(xml,"canon");
                cb.sort_with(compare_cb);
                claws &L = coerce( *canon );

                //--------------------------------------------------------------
                // expanding constraints
                //--------------------------------------------------------------
                {
                    size_t i = 0;
                    for(const collector::entry *ep=cb->head;ep;ep=ep->next)
                    {
                        claw                      *cl = L.push_back( new claw(++i) );
                        const cxx_array<unsigned> &cf = *ep;
                        for(const sp_gnode *sn=breed->conserved->head;sn;sn=sn->next)
                        {
                            const anode     &an = ***sn;
                            const size_t     J  = *an;
                            const unsigned   w  = cf[J];
                            if(w) {
                                const species   &sp = an.host;
                                //std::cerr << "\t+" << sp.name << " * " << cf << std::endl;
                                cl->add(sp,w);
                            }
                        }
                        YACK_XMLOG(xml, cf << " => 0=d" << *cl);
                    }
                }
                assert(cb->size==canon->size);
                
                //--------------------------------------------------------------
                // grouping constraints
                //--------------------------------------------------------------
                const size_t Nq = canon->size;
                matrix<bool> attached(Nq,Nq);
                for(const claw *node=L.head;node;node=node->next)
                {
                    const size_t i = **node;
                    attached[i][i] = true;
                    for(const claw *scan=node->next;scan;scan=scan->next)
                    {
                        const size_t j = **scan; assert(i!=j);
                        attached[i][j] = attached[j][i] = scan->attached_to(*node);
                    }
                }
                std::cerr << "attached=" << attached << std::endl;
            }



        }

    }

}

