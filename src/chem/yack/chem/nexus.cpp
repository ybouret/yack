
#include "yack/chem/nexus.hpp"
#include "yack/system/imported.hpp"
#include "yack/apex/alga.hpp"
#include "yack/math/iota.hpp"

namespace yack
{
    using namespace math;
    
    namespace chemical
    {
        
        const char * const nexus:: clid = "[nexus]";
        bool              &nexus::verbose = entity::verbose;
        
        
        nexus:: ~nexus() throw()
        {
        }
        
        nexus:: nexus(const library    &lib,
                      const equilibria &eqs,
                      const double      t) :
        corelib( lib ),
        singles( eqs ),
        lattice( singles ),
        next_en( NULL ),
        working( corelib.head() ),
        M( corelib.size() ),
        N( singles.size() ),
        L(0),
        Nu(N,N>0?M:0),
        worklib( corelib ),
        
        
        // species
        crit(M),
        
        // singles
        K( N ),
        regular(),
        roaming(),
        related(),

        // c-laws
        Nq(0),
        Qm(),

        euid(),
        
        lockLib(coerce(corelib)),
        lockEqs(coerce(singles))
        {
            const xmlog xml(clid,std::cerr,verbose);
            YACK_XMLSUB(xml, "initialize");

            //------------------------------------------------------------------
            //
            // initialize criterions for species
            //
            //------------------------------------------------------------------
            assert(corelib.size()==worklib.size());
            for(const snode *sn=corelib.head();sn;sn=sn->next)
            {
                const species &s = ***sn;
                const size_t   j = *s;
                coerce(crit[j]) = (s.rank<=0) ? spectator : conserved;
            }

            //------------------------------------------------------------------
            //
            // initialize singles, their properties and their
            // impact on species
            //
            //------------------------------------------------------------------
            if(N>0)
            {
                YACK_XMLSUB(xml, "singles");
                for(const enode *en=singles.head();en;en=en->next)
                {
                    const equilibrium &eq = ***en;
                    const size_t       ei = *eq;
                    YACK_XMLOG(xml,eq);

                    //----------------------------------------------------------
                    // check validity
                    //----------------------------------------------------------
                    if(!eq.neutral()) throw imported::exception(clid,"<%s> is not neutral", eq.name() );
                    if(!eq.minimal()) throw imported::exception(clid,"<%s> is not minimal", eq.name() );

                    //----------------------------------------------------------
                    // prepare topology
                    //----------------------------------------------------------
                    eq.fill(coerce(Nu[ei]));

                    //----------------------------------------------------------
                    // classification
                    //----------------------------------------------------------
                    switch(eq.kind)
                    {
                        case undefined: throw imported::exception(clid,"<%s> is undefined", eq.name() );
                        case both_ways:
                            coerce(regular) << &eq;
                            break;
                            
                        case join_only:
                        case part_only:
                            coerce(roaming) << &eq;
                            for(const cnode *cn=eq.head();cn;cn=cn->next)
                            {
                                const species &s = ****cn;
                                coerce(crit[*s]) = unbounded;
                            }
                            break;
                    }

                    //----------------------------------------------------------
                    // setup known constants
                    //----------------------------------------------------------
                    K[ei] = eq.K(t);
                }

                //----------------------------------------------------------
                //
                // checking rank from full topology
                //
                //----------------------------------------------------------
                const size_t rank =  alga::rank(Nu);
                YACK_XMLOG(xml, "-- Nu   = " << Nu);
                YACK_XMLOG(xml, "-- rank = " << rank << " / " << N);
                if(rank<N) throw imported::exception(clid,"detected dependent equilibria");
                
                singles.graphviz("singles.dot", corelib);


                //--------------------------------------------------------------
                //
                // build related (at least one cluster)
                //
                //----------------------------------------------------------
                build_related(xml); assert(related.size>0);

                //--------------------------------------------------------------
                //
                // make conserved sets within clusters
                //
                //--------------------------------------------------------------
                conserved_set(xml);

                //--------------------------------------------------------------
                //
                // make manifold
                //
                //--------------------------------------------------------------
                make_manifold(xml);
                
                std::cerr << "lattice=" << lattice << std::endl;
                
                //--------------------------------------------------------------
                //
                // update lattice
                //
                //--------------------------------------------------------------
                coerce(L) = lattice.size();
                coerce(topo) = new imatrix(L,M);
                coerce(tbal) = new imatrix(L,M);
                {
                    const enode *en = lattice.head();
                    for(size_t i=N;i>0;--i)
                    {
                        assert(en);
                        en=en->next;
                        iota::load(coerce(*topo)[i],Nu[i]);
                    }
                    coerce(next_en) = en;
                }

#if 1
                for(const enode *en=next_en;en;en=en->next)
                {
                    const equilibrium &eq = ***en;
                    const size_t       ei = *eq;
                    eq.fill(coerce(*topo)[ei]);
                }

                coerce(*tbal).assign(*topo);
                for(size_t i=L;i>0;--i)
                {
                    writable<int> &r = coerce(*tbal)[i];
                    for(size_t j=M;j>0;--j)
                    {
                        if( crit[j] != conserved )
                        {
                            r[j] = 0;
                        }
                    }
                }
#endif
                build_eqs_uid();
            }
            
        }

        void  nexus:: build_eqs_uid()
        {
            const size_t  width = lattice.maxlen;
            coerce(euid)        = new cmatrix(L,width+1);
            cmatrix     &uid    = coerce(*euid);
            for(const enode *en=lattice.head();en;en=en->next)
            {
                const equilibrium &eq = ***en;
                const size_t       ei = *eq;
                const string      &id = eq.name; assert(id.size()<=width);
                const size_t       nc = id.size();
                writable<char>    &ch = uid[ei];
                for(size_t i=1;i<=nc;++i)       ch[i] = id[i];
                for(size_t i=nc+1;i<=width;++i) ch[i] = ' ';
            }
            display_vert_euid(std::cerr,0);

        }

        void nexus:: display_vert_euid(std::ostream &os, const size_t space) const
        {
            const cmatrix &id = *euid;
            const size_t   nr = lattice.maxlen;
            const size_t   nc = L;
            for(size_t i=1;i<=nr;++i)
            {
                for(size_t j=space;j>0;--j) os << ' ';
                os << '|';
                for(size_t j=1;j<=nc;++j) os << id[j][i] << '|';
                os << std::endl;
            }

        }



        void nexus:: upgrade_lattice(writable<double> &Kl)
        {
            assert(Kl.size()>=L);
            iota::save(Kl,K);
            for(const enode *en=next_en;en;en=en->next)
            {
                const equilibrium &eq = ***en;
                const size_t       ei = *eq;
                Kl[ei] = eq.K(-1);
            }
        }
        
        void nexus:: compute_singles(const double t)
        {
            for(const enode *en=singles.head();en;en=en->next)
            {
                const equilibrium &eq = ***en;
                K[*eq] = eq.K(t);
            }
        }

        static inline
        bool are_detached(const readable<int> &lhs, const readable<int> &rhs) throw()
        {
            assert(lhs.size()==rhs.size());
            for(size_t k=lhs.size();k>0;--k)
            {
                if(lhs[k]!=0 && rhs[k]!=0) return false;
            }
            return true;
        }
        
        void nexus:: build_detached(matrix<bool> &detached, const imatrix &coef) const
        {
            assert( detached.rows == coef.rows );
            assert( detached.cols == coef.rows );
            
            detached.ld(false);
            for(size_t i=coef.rows;i>0;--i)
            {
                writable<bool>      &d_i = detached[i];
                const readable<int> &c_i = coef[i];
                for(size_t j=i-1;j>0;--j)
                {
                    const readable<int> &c_j = coef[j];
                    d_i[j] = detached[j][i] = are_detached(c_i,c_j);
                }
            }
        }

    }
    
}

