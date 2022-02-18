#include "yack/chem/plexus.hpp"
#include "yack/exception.hpp"
#include "yack/math/tao/v3.hpp"
#include "yack/sort/sum.hpp"

#include <cmath>

namespace yack
{
    using namespace math;

    namespace chemical
    {
        bool plexus::verbose = false;

        plexus:: ~plexus() throw() {}

        const char plexus::clid[] = "chemical::plexus";

        static inline
        size_t check(const library &lib, const equilibria &eqs)
        {
            if(lib.size()<eqs.size())
            {
                throw exception("%s: too many equilibria",plexus::clid);
            }
            return lib.size();
        }

        plexus:: plexus(library &lib_, equilibria &eqs_) :
        lib(lib_),
        eqs(eqs_),
        N(eqs.size()),
        M(check(lib,eqs)),
        A(lib.active()),
        ntab(10,N),
        mtab(10,M),

        K(ntab.next()),
        Gamma(ntab.next()),

        Corg(mtab.next()),
        Ctry(mtab.next()),
        Ctmp(mtab.next()),
        dC(mtab.next()),

        xi(ntab.next()),
        sc(ntab.next()),

        Nu(N, (N>0) ? M : 0),
        NuT(Nu.cols,Nu.rows),
        Psi(Nu.rows,Nu.cols),
        W(N,N),
        blocked(N,false),
        
        rstack(M,as_capacity),
        ustack(M,as_capacity),
        LU(N),
        clusters(),

        lib_lock( coerce(lib) )
        {
            YACK_CHEM_PRINTLN("#species=" << M );
            YACK_CHEM_PRINTLN("#active =" << A );
            YACK_CHEM_PRINTLN("#eqs    =" << N );

            //------------------------------------------------------------------
            //
            // initialize Nu
            //
            //------------------------------------------------------------------
            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                eq.validate();
                eq.fill(coerce(Nu[*eq]));
            }

            //------------------------------------------------------------------
            //
            // finalize
            //
            //------------------------------------------------------------------
            coerce(NuT).assign(Nu,transposed);

            if(N>0)
            {
                //--------------------------------------------------------------
                //
                // check equilibria independence
                //
                //--------------------------------------------------------------
                {
                    matrix<apq> G(N,N);
                    tao::v3::gram(G,Nu);
                    YACK_CHEM_PRINTLN( "Nu  = " << Nu );
                    YACK_CHEM_PRINTLN( "NuT = " << NuT);
                    YACK_CHEM_PRINTLN( "G   = " << G  );

                    lu<apq> alu(N);
                    if(!alu.build(G))
                        throw exception("%s: dependant equilibria detected",clid);
                }


                //--------------------------------------------------------------
                //
                // create groups
                //
                //--------------------------------------------------------------
                {
                    list_of<cluster> &groups = coerce(clusters);
                    for(const enode *node=eqs.head();node;node=node->next)
                    {
                        const equilibrium &eq    = ***node;
                        bool               found = false;
                        for(cluster *cls=groups.head;cls;cls=cls->next)
                        {
                            if(cls->connected_to(eq))
                            {
                                (*cls) << &eq;
                                found = true;
                                break;
                            }
                        }

                        if(!found)
                        {
                            groups.push_back( new cluster(groups.size+1,eq,eqs.width) );
                        }
                    }
                    for(cluster *cls=groups.head;cls;cls=cls->next)
                    {
                        cls->finalize(lib.head());
                    }
                }
            }

            if(verbose)
            {
                std::cerr << "// <clusters count='" << clusters.size << "'>" << std::endl;
                for(const cluster *cls=clusters.head;cls;cls=cls->next)
                {
                    std::cerr << *cls << std::endl;
                }
                std::cerr << "// <clusters/>" << std::endl;
            }

            

        }

        void plexus:: computeK(const double t)
        {
            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                eq(K) = eq.K(t);
            }
        }

        void plexus:: computeGamma(const readable<double> &C) throw()
        {
            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       i  = *eq;
                Gamma[i] = eq.mass_action(K[i],C);
            }
        }

        void plexus:: computeGammaAndPsi(const readable<double> &C) throw()
        {
            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       i  = *eq;
                Gamma[i] = eq.drvs_action(Psi[i],K[i],C);
            }
        }


        const readable<double> &plexus:: make_trial(const double u) throw()
        {
            for(const snode *node=lib.head();node;node=node->next)
            {
                const species &sp = ***node;
                const size_t   j  = *sp;
                Ctry[j] = max_of<double>(0,Corg[j]+u*dC[j]);
            }
            return Ctry;
        }

        const readable<double> & plexus:: make_trial(const double u, const readable<size_t> &vanishing) throw()
        {
            make_trial(u);
            for(size_t k=vanishing.size();k>0;--k)
            {
                Ctry[ vanishing[k] ] = 0;
            }
            return Ctry;
        }

        void plexus:: save_profile(const char *filename, const double umax)
        {
            ios::ocstream fp(filename);
            YACK_CHEM_PRINTLN("// [saving " << filename << "] -> " << umax);
            for(double u=0;u<1;u+=0.0001)
            {
                const double xx = u * umax;
                fp("%.15g %.15g\n", xx, (*this)(xx) );
            }
        }


    }
    
}

