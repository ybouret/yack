#include "yack/chem/plexus.hpp"
#include "yack/exception.hpp"
#include "yack/math/tao/v3.hpp"
#include "yack/sort/sum.hpp"
#include "yack/ptr/auto.hpp"

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
        active(),
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
        xs(ntab.next()),
        
        Nu(N, (N>0) ? M : 0),
        NuT(Nu.cols,Nu.rows),
        Psi(Nu.rows,Nu.cols),
        W(N,N),
        blocked(N,false),
        
        rstack(M,as_capacity),
        ustack(M,as_capacity),
        LU(N),

        lib_lock( coerce(lib) ),
        eqs_lock( coerce(eqs) )
        {
            YACK_CHEM_PRINTLN("#species=" << M );
            YACK_CHEM_PRINTLN("#active =" << A );
            YACK_CHEM_PRINTLN("#eqs    =" << N );

            //------------------------------------------------------------------
            //
            // initialize active
            //
            //------------------------------------------------------------------
            for(const snode *node=lib.head();node;node=node->next)
            {
                const species &sp = ***node;
                if(sp.rank>=1)
                {
                    coerce(active) << &sp;
                }
            }


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

                computeConn();
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


        double plexus:: gammaVariance() throw()
        {
            for(size_t i=N;i>0;--i)
            {
                sc[i] = squared(Gamma[i]);
            }

            return sorted::sum(sc,sorted::by_value) / N;
        }

        double plexus:: computeVariance(const readable<double> &C)
        {
            if(N>0)
            {
                computeGamma(C);
                return gammaVariance();
            }
            else
            {
                return 0;
            }

        }

        double plexus:: operator()(const double u)
        {
            make_trial(u);
            return computeVariance(Ctry);
        }


    }
    
}

