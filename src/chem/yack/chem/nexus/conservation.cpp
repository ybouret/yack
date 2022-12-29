
#include "yack/chem/nexus.hpp"
#include "yack/math/iota.hpp"
#include "yack/raven/qbranch.hpp"
#include "yack/math/algebra/ortho-family.hpp"
#include "yack/apex/alga.hpp"
#include "yack/sequence/cxx-series.hpp"
#include "yack/sequence/bunch.hpp"
#include "yack/randomized/rand.hpp"

#include <iomanip>

namespace yack
{
    using namespace math;
    using namespace raven;
    
    namespace chemical
    {
        
        namespace {
            
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
            
            
            
            
            class collector : public bunch<unsigned>
            {
            public:
                const xmlog &xml;
                
                explicit collector(const size_t dim, const xmlog &_) :
                bunch<unsigned>(dim),
                xml(_)
                {
                }
                
                virtual ~collector() throw()
                {
                }
                
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
        
        void nexus:: conserved_set_(cluster &sharing, const xmlog &xml)
        {
            static const char * const fn = "conserved_subset";
            YACK_XMLSUB(xml,fn);
            eq_team      usual;
            sp_list      house;
            
            //------------------------------------------------------------------
            //
            // extract usual equilibria and house of species
            //
            //------------------------------------------------------------------
            {
                addrbook     tribe;
                
                for(const eq_node *en=sharing.head;en;en=en->next)
                {
                    const equilibrium &eq = **en;
                    if( genus->bounded.met(eq) ) {
                        usual << &eq;
                        eq.update(tribe);
                    }
                }
                
                for(addrbook::const_iterator it=tribe.begin();it!=tribe.end();++it)
                {
                    const species &sp = *static_cast<species*>(*it);
                    if( conserved != crit[*sp] ) continue;;
                    house << static_cast<species *>(*it);
                }
                house.sort();
            }
            
            //------------------------------------------------------------------
            //
            // study result
            //
            //------------------------------------------------------------------
            YACK_XMLOG(xml, "-- #usual = " << usual.size << " / " << sharing.size << ", #house=" << house.size);
            if(usual.size<=0) return;
            if(verbose)
            {
                for(const eq_node *node=usual.head;node;node=node->next)
                {
                    *xml << " (*) " << (**node).name << std::endl;
                }
                *xml << "  |_" << house << std::endl;
            }
            assert(house.size>=2);
            
            
            //------------------------------------------------------------------
            //
            // prepare answer
            //
            //------------------------------------------------------------------
            const size_t n = usual.size;
            const size_t m = house.size;
            switch( __sign::of(m,n) )
            {
                case negative: throw imported::exception(fn,"too few species");
                case __zero__:
                    YACK_XMLOG(xml, "-- specific system");
                    return;
                case positive:
                    break;
            }
            collector cb(m,xml);
            
            {
                //--------------------------------------------------------------
                //
                // build virtual topology
                //
                //--------------------------------------------------------------
                assert(n<m);
                const size_t ker = m-n;
                imatrix      nu(n,m);
                {
                    size_t i=1;
                    for(const eq_node *en=usual.head;en;en=en->next,++i)
                    {
                        const readable<int> &Nu_i = Nu[***en];
                        writable<int>       &nu_i = nu[i];
                        size_t j=1;
                        for(const sp_node *sn=house.head;sn;sn=sn->next,++j) {
                            nu_i[j] = Nu_i[ ***sn ];
                        }
                    }
                }
                usual.release();
                YACK_XMLOG(xml,"-- nu = " << nu);
                
                //--------------------------------------------------------------
                //
                // build compressed orthogonal space
                //
                //--------------------------------------------------------------
                imatrix Q;
                {
                    imatrix Q0(m,m);
                    if(!ortho_family::build(Q0,nu,true))    throw imported::exception(fn,"singular sub-system");
                    compressQ(Q,Q0); if(ker!=alga::rank(Q)) throw imported::exception(fn,"singular compressed sub-system");
                }
                YACK_XMLOG(xml,"-- Q  = " << Q);
                
                //--------------------------------------------------------------
                //
                // apply RAVEn from root
                //
                //--------------------------------------------------------------
                {
                    YACK_XMLSUB(xml, "RAVEn");
                    qbranch  source;
                    source.batch(Q,ker,cb);
                }
            }
            
            //------------------------------------------------------------------
            //
            // use collected entries
            //
            //------------------------------------------------------------------
            if(cb->size)
            {
                conservation_laws &canon = coerce(*sharing.canon);
                size_t ic = 0;
                for(const collector::entry *ep=cb->head;ep;ep=ep->next)
                {
                    const readable<unsigned> &cf   = *ep;
                    conservation_law         &claw = *(canon.push_back( new conservation_law() ));
                    size_t j=1;
                    for(const sp_node *sn=house.head;sn;sn=sn->next,++j)
                    {
                        const unsigned w = cf[j];
                        if(w)
                            claw(**sn,w);
                    }
                    claw.finalize(++ic);
                    YACK_XMLOG(xml,"--> " << claw);
                }
            }
            house.release();
            
            //------------------------------------------------------------------
            //
            // create teams of c-laws for this cluster
            //
            //------------------------------------------------------------------
            for(const conservation_law *claw=sharing.canon->head;claw;claw=claw->next)
            {
                coerce(*sharing.cells).recruit(*claw);
            }
            YACK_XMLOG(xml,"|_#cell = " << sharing.cells->size );
        }
        
        
        void nexus:: conserved_set(const xmlog &xml)
        {
            static const char * const fn = "conserved_set";
            YACK_XMLSUB(xml,fn);
            for(cluster *sharing=wired.head;sharing;sharing=sharing->next)
            {
                conserved_set_(*sharing,xml);
                coerce(Nq) += sharing->canon->size;
            }
            YACK_XMLOG(xml,"-- conservation  laws : " << Nq);
            YACK_XMLOG(xml,"-- equilibria         : " << singles.size() );
            YACK_XMLOG(xml,"-- committed species  : " << genus->committed.size  );
            
            if(Nq)
            {
                
                
                coerce(Qm).make(Nq,M);
                {
                    size_t i=1;
                    for(const cluster *cls=wired.head;cls;cls=cls->next)
                    {
                        for(const conservation_law *claw = cls->canon->head;claw;claw=claw->next,++i)
                        {
                            claw->fill( coerce(Qm[i]) );
                        }
                    }
                }

                std::cerr << "\tQm=" << Qm << std::endl;
                
                
                if(Nq+N<M)
                {
                    std::cerr << "Trying to raven..." << std::endl;
                    raven::qmatrix F(M,M);
                    for(size_t i=1;i<=N;++i)
                    {
                        if(!F(Nu[i])) throw exception("bad Nu");
                    }
                    for(size_t i=1;i<=Nq;++i)
                    {
                        if(!F(Qm[i])) throw exception("bad Qm");
                    }
                    std::cerr << "F=" << F << std::endl;
                    
                    cxx_array<int>    dum(M);
                    randomized::rand_ ran;
                    while(true)
                    {
                        for(size_t i=M;i>0;--i) dum[i] = static_cast<int>( ran.in(-10,10) );
                        if(F(dum)) break;
                        std::cerr << "invalid dummy=" << dum << std::endl;
                    }
                    std::cerr << "F=" << F << std::endl;
                }
            }
            
        }
        
        
    }
    
}

