
#include "yack/chem/plexus.hpp"
#include "yack/chem/lua/equilibria.hpp"
#include "yack/utest/run.hpp"
#include "yack/system/env.hpp"
#include "yack/counting/comb.hpp"

using namespace yack;
using namespace chemical;

static inline double f_abs(const double x)
{
    return fabs(x);
}

static inline double d_abs(const double x)
{
    return x < 0 ? -1 : ( 0 < x ? 1 : 0);
}

double get_dif(const double omega, const readable<int> &A, const readable<int> &B, double &dd)
{
    double d = 0;
    for(size_t i=A.size();i>0;--i)
    {
        const double y  = A[i];
        const double dx = A[i]+B[i];
        const double dv = y - omega * dx;
        dd -= dx * d_abs(dv);
        d  += fabs(dv);
    }
    return d;
}

double get_sum(const double omega, const readable<int> &A, const readable<int> &B, double &ds)
{
    double s = 0;
    for(size_t i=A.size();i>0;--i)
    {
        const double y  = A[i];
        const double sx = A[i]-B[i];
        const double sv = y - omega * sx;
        s  += f_abs(sv);
        ds -= sx * d_abs(sv);
    }
    return s;
}



static inline void delta_NU(const readable<int> &A, const readable<int> &B)
{
    std::cerr << "A=" << A << std::endl;
    std::cerr << "B=" << B << std::endl;
    ios::ocstream fp("delta.dat");
    for(double omega=0;omega<1;omega+=0.01)
    {
        double       dd = 0;
        const double d  = get_dif(omega,A,B,dd);
        double       ds = 0;
        const double s  = get_sum(omega,A,B,ds);

        fp("%.15g %.15g %.15g %.15g %.15g\n",omega,d,dd,s,ds);
    }

    {
        const double omega  = 1.0;
        double       dd = 0;
        const double d  = get_dif(omega,A,B,dd);
        double       ds = 0;
        const double s  = get_sum(omega,A,B,ds);

        fp("%.15g %.15g %.15g %.15g %.15g\n",omega,d,dd,s,ds);
    }

}

static inline void try_solve(plexus &cs, writable<double> &C)
{
    
    cs.lib(std::cerr << "Cini=",C);
    cs.solve(C);
    cs.lib(std::cerr << "Cend=",C);
    cs.eqs(std::cerr << "Gamma=",cs.Gamma);
}

YACK_UTEST(plexus)
{
    //concurrent::single::verbose = true;
    
    randomized::rand_ ran;
    library           lib;
    lua_equilibria    eqs;

    eqs->dostring("function K(t) return 1e-3*(1.1+math.cos(t)); end");

    for(int i=1;i<argc;++i)
    {
        eqs(argv[i],lib);
    }

    std::cerr << "lib=" << lib << std::endl;
    std::cerr << "eqs=" << eqs << std::endl;


    if( environment::flag("YACK_CHEM_VERBOSE") )
    {
        plexus::verbose = true;
        //math::minimize::verbose = true;
        //math::bracket::verbose  = true;
    }

    plexus cs(lib,eqs);

    cs.computeK(0);
    std::cerr << "K=" << cs.K << std::endl;

    cs.gv("plexus.dot");

    if(cs.N>1)
    {
        delta_NU(cs.Nu[1],cs.Nu[2]);
    }


    if(cs.N)
    {
        vector<double> C(lib.size(),0);
        try
        {
            //----------------------------------------------------------------------
            // from 0
            //----------------------------------------------------------------------
            lib.fill(C,0.7,ran);
            try_solve(cs,C);
            //std::cerr << "Nu=" << cs.Nu << std::endl;

            return 0;

            //----------------------------------------------------------------------
            // loading active
            //----------------------------------------------------------------------
            vector<species *> spec;
            for(const snode *node=lib.head();node;node=node->next)
            {
                const species &s = ***node;
                if(s.rank>0)
                {
                    spec.push_back( (species *)&s );
                }
            }
            YACK_CHECK(spec.size() == cs.A );

            for(size_t na=1;na<=spec.size();++na)
            {
                combination       comb(spec.size(),na);
                vector<species *> sub(na,NULL);
                do
                {
                    std::cerr << "-- using";
                    for(size_t i=1;i<=na;++i)
                    {
                        sub[i] = spec[ comb[i] ];
                        std::cerr << " " << *sub[i];
                    }
                    std::cerr << std::endl;

                    for(size_t iter=0;iter<1;++iter)
                    {
                        C.ld(0);
                        for(size_t i=1;i<=na;++i)
                        {
                            const species &s = *sub[i];
                            const size_t   j = *s;
                            C[j] = species::concentration(ran);
                        }
                        try_solve(cs,C);
                    }

                } while( comb.next() );
            }




        }
        catch(const exception &e)
        {
            e.show(std::cerr);
            std::cerr << "C=" << C << std::endl;
        }
    }



}
YACK_UDONE()
