#include "yack/chem/plexus.hpp"
#include "yack/exception.hpp"
#include "yack/math/iota.hpp"
#include "yack/apex.hpp"

namespace yack
{
    using namespace math;
    
    namespace chemical
    {
        
        const char   plexus::clid[]  = "chemical::plexus";
        const char   plexus::vpfx[]  = "//  ";
        const bool & plexus::verbose = entity::verbose;
        
        plexus:: plexus(library     &lib_,
                        equilibria  &eqs_,
                        const double tini) :
        lib( lib_ ),
        sub( lib  ),
        singles( eqs_ ),
        couples(),
        lattice(),
        
        M( lib.size() ),
        N( singles.size() ),
        
        Nc(0),
        Nl(0),
        
        ntab(10,N),
        mtab(10,M),
        ltab(10,Nl),
        
        active(),
        
        Nu(N,N>0?M:0),
        NuT(Nu.cols,Nu.rows),
        
        K( ntab.next() ),
        Xtry( ntab.next() ),
        
        Corg( mtab.next() ),
        Ctry( mtab.next() ),
        Cend( mtab.next() ),
        
        Kl( ltab.next() ),
        Xl( ltab.next() ),
        Cs(),

        cls(),

        libLock(lib_),
        eqsLock(eqs_)
        {
            YACK_CHEM_MARKUP( vpfx, "ChemicalPlexus");
            YACK_CHEM_PRINTLN("M  = " << M);
            YACK_CHEM_PRINTLN("N  = " << N);
            
            //------------------------------------------------------------------
            //
            // build active species
            //
            //------------------------------------------------------------------
            for(const snode *node=lib.head();node;node=node->next)
            {
                const species &s = ***node;
                if(s.rank>0)
                {
                    coerce(active) << &s;
                }
            }
            
            //------------------------------------------------------------------
            //
            // build Nu and initialize constants
            //
            //------------------------------------------------------------------
            equilibrium::display_time = tini;
            {
                YACK_CHEM_PRINTLN(vpfx << "-------- singles --------" << std::endl << singles);
                for(const enode *node=singles.head();node;node=node->next)
                {
                    const equilibrium &eq = ***node;
                    if(!eq.is_minimal()) throw exception( "%s is not minimal", eq.name() );
                    if(!eq.is_neutral()) throw exception( "%s is not neutral", eq.name() );
                    eq.fill( coerce(Nu[*eq]) );
                    K[*eq] = eq.K(tini);
                }
            }
            coerce(NuT).assign(Nu,transposed);
            YACK_CHEM_PRINTLN("Nu   = " << Nu);
            YACK_CHEM_PRINTLN("NuT  = " << NuT);
            YACK_CHEM_PRINTLN("K    = " << K);
            
            //------------------------------------------------------------------
            //
            // check indep equilibria
            //
            //------------------------------------------------------------------
            if(N>0)
            {
                matrix<apq> G(N,N);
                lu<apq>     alu(N);
                iota::gram(G,Nu);
                YACK_CHEM_PRINTLN("gram = " <<G);
                if( !alu.build(G) ) throw exception("%s: dependent equilibria",clid);
            }
            
            //------------------------------------------------------------------
            //
            // build all couples
            //
            //------------------------------------------------------------------
            coerce(Nc) = buildMatchingCouples();
            
            //------------------------------------------------------------------
            //
            // build lattice of all singles+couples
            //
            //------------------------------------------------------------------
            duplicateIntoLattice(singles);
            duplicateIntoLattice(couples);
            coerce(Nl) = lattice.size();
            YACK_CHEM_PRINTLN(vpfx << "-------- lattice --------" << std::endl << lattice);
            
            if(Nl>0)
            {
                ltab.make(Nl);         // linear memory
                Cs.make(Nl,M);         // solution C
                assert(Nl==Kl.size()); // check
                assert(Nl==Xl.size()); // check


                //--------------------------------------------------------------
                // first time Kl computation
                //--------------------------------------------------------------
                iota::save(Kl,K);
                for(const enode *node=couples.head();node;node=node->next)
                {
                    const equilibrium &eq = ***node;
                    const size_t       ei = *eq;
                    Kl[ei] = eq.K(tini); // from K
                }
                YACK_CHEM_PRINTLN("K  = " << K);
                YACK_CHEM_PRINTLN("Kl = " << Kl);
                {
                    YACK_CHEM_MARKUP( vpfx, "Orthogonality");
                    for(const enode *node = lattice.head(); node; node=node->next )
                    {
                        const equilibrium &central = ***node;
                        for(const enode *scan = lattice.head();scan;scan=scan->next)
                        {
                            if(scan==node) continue;
                            const equilibrium &replica = ***scan;
                            if(central.are_detached_from(replica))
                            {
                                lattice.pad(std::cerr << central.name, central) << " _|_ " << replica.name << std::endl;
                            }
                        }
                    }
                }


                //--------------------------------------------------------------
                // building clusters
                //--------------------------------------------------------------
                {
                    YACK_CHEM_MARKUP( vpfx, "buildingClusters");
                    for(const enode *node = lattice.head(); node; node=node->next )
                    {
                        // get equilibrium, an start a cluster with it
                        const equilibrium &eq = ***node;
                        cluster           &cl = *coerce(cls).push_back( new cluster() );
                        cl << &eq;
                        YACK_CHEM_PRINTLN(vpfx << "  @" << eq.name);

                        // try all other equilbria
                        for(const enode *scan =node->next;scan;scan=scan->next)
                        {
                        }
                    }
                }
                
                
            }
            
            
        }
        
        
        void plexus:: computeK(const double t)
        {
            // singles, fill K and Kl[1..N]
            for(const enode *node=singles.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       ei = *eq;
                K[ei] = Kl[ei] = eq.K(t);
            }
            
            // couples, fill Kl[N+1..Nl]
            for(const enode *node=couples.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       ei = *eq;
                Kl[ei] = eq.K(t); // computed from K
            }
            
        }
        
        void   plexus:: duplicateIntoLattice(const equilibria &src)
        {
            for(const enode *eq = src.head();eq;eq=eq->next)
                coerce(lattice).add( **eq );
        }
        
        
        plexus:: ~plexus() throw()
        {
        }
        
        
        
        
    }
}
