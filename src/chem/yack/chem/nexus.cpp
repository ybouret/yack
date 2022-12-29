
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
        M( corelib.size() ),
        N( singles.size() ),
        L(0),
        Nu(N,N>0?M:0),
        worklib( corelib ),
        
        
        // species
        crit(M),
        
        // singles
        K( N ),
        genus( new eq_tier() ),
        wired(),

        // c-laws
        Nq(0),
        Qm(),


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
                            coerce(genus->bounded) << &eq;
                            break;
                            
                        case join_only:
                        case part_only:
                            coerce(genus->roaming) << &eq;
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

                //--------------------------------------------------------------
                //
                // compiling genus
                //
                //--------------------------------------------------------------
                coerce(genus)->compile();
                YACK_XMLOG(xml,"-- unbridled : " << genus->unbridled);
                YACK_XMLOG(xml,"-- regulated : " << genus->regulated);
                YACK_XMLOG(xml,"-- committed : " << genus->committed);

                //----------------------------------------------------------
                //
                // checking rank from full topology
                //
                //----------------------------------------------------------
                const size_t rank =  alga::rank(Nu);
                YACK_XMLOG(xml, "-- Nu   = " << Nu);
                YACK_XMLOG(xml, "-- rank = " << rank << " / " << N);
                if(rank<N) throw imported::exception(clid,"detected dependent equilibria");
                


                //--------------------------------------------------------------
                //
                // build wired (int at least one cluster)
                //
                //--------------------------------------------------------------
                resolve_wired(xml); assert(wired.size>0);

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


                YACK_XMLOG(xml,"lattice=" << lattice);

                
                //--------------------------------------------------------------
                //
                // finalize
                //
                //--------------------------------------------------------------
                coerce(L) = lattice.size();
                imatrix      topology(L,M);
                matrix<bool> detached(L,L);

                //--------------------------------------------------------------
                // fill global topology and setup next_en
                //--------------------------------------------------------------
                {
                    const enode *en = lattice.head();
                    for(size_t i=N;i>0;--i)
                    {
                        assert(en);
                        en=en->next;
                        iota::load(topology[i],Nu[i]);
                    }
                    coerce(next_en) = en;
                }

                //--------------------------------------------------------------
                // complete global topology and apply to clusters
                //--------------------------------------------------------------
                for(const enode *en=next_en;en;en=en->next)
                {
                    const equilibrium &eq = ***en;
                    const size_t       ei = *eq;
                    eq.fill(topology[ei]);
                }

                build_detached(detached,topology);
                for(cluster *cc=wired.head;cc;cc=cc->next)
                    cc->build_army_with(detached,xml);

                //--------------------------------------------------------------
                // update topology by removing not conserved species
                // and apply to clusters
                //--------------------------------------------------------------
                for(size_t i=L;i>0;--i)
                {
                    writable<int> &r = topology[i];
                    for(size_t j=M;j>0;--j)
                    {
                        if( crit[j] != conserved )
                        {
                            r[j] = 0;
                        }
                    }
                }

                build_detached(detached,topology);
                for(cluster *cc=wired.head;cc;cc=cc->next)
                    cc->build_wing_with(detached,xml);
                
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

        void nexus:: print_detached(const matrix<bool> &detached) const
        {
            for(const enode *en=lattice.head();en;en=en->next)
            {
                const equilibrium    &eq = ***en;
                const size_t          ei = *eq;
                const readable<bool> &di = detached[ei];
                lattice.pad(std::cerr << eq.name,eq) << " : " << di << "_|_{ ";
                for(const enode *sub=lattice.head();sub;sub=sub->next)
                {
                    const equilibrium &ep = ***sub;
                    const size_t       ej = *ep;
                    if(di[ej]) std::cerr << ep.name << ' ';
                }
                std::cerr << "}" << std::endl;
            }
        }


    }
    
}

