#include "yack/chem/reactor.hpp"
#include "yack/ios/xmlog.hpp"
#include "yack/type/utils.hpp"
#include "yack/math/opt/optimize.hpp"
#include <iomanip>


namespace yack
{
    using namespace math;

    namespace chemical
    {

        double reactor:: Balance(const readable<double> &C)
        {
            Bal.free();
            for(const anode *node=working.head;node;node=node->next)
            {
                const size_t j = ***node;
                const double c = C[j];
                if(c<0) Bal << squared(c);
            }
            return xadd.tableau(Bal)/2;
        }

        double reactor:: Balance(const double u)
        {
            for(const anode *node=working.head;node;node=node->next)
            {
                const size_t j = ***node;
                Ctry[j] = Corg[j] + u * dC[j];
            }
            return Balance(Ctry);
        }


        struct callB
        {
            reactor &self;

            double operator()(double u)
            {
                return self.Balance(u);
            }
        };
        

        bool reactor:: balance(writable<double> &C0)
        {
            static const char fn[] = "[reactor]";
            const xmlog       xml(fn,std::cerr,entity::verbose);
            YACK_XMLSUB(xml,"Balancing");
            if(verbose) corelib(*xml << "-- Cini=","", C0);

            if(N<=0)
            {
                YACK_XMLOG(xml,"-- no equilibrium");
                return true;
            }

            for(size_t j=M;j>0;--j)
            {
                Corg[j] = Cend[j] = Ctry[j]  = C0[j];
                dC[j]   = 0;
            }

            //------------------------------------------------------------------
            //
            // initial primary balancing
            //
            //------------------------------------------------------------------
            {
                YACK_XMLSUB(xml,"PrimaryBalancing");
                bool primaryBalanced = true;
                for(const enode *node=singles.head();node;node=node->next)
                {
                    const equilibrium &eq = ***node;
                    YACK_XMLOG(xml, "--> " << eq.name);
                    if(eq.try_primary_balance(Corg,xml))
                    {
                        YACK_XMLOG(xml, "[+] " << eq.name);
                    }
                    else
                    {
                        YACK_XMLOG(xml, "[-] " << eq.name);
                        primaryBalanced = false;
                    }
                }
                if(!primaryBalanced)
                {
                    YACK_XMLOG(xml, "-- [[ FAILURE ]]");
                    return false;
                }
            }


            if(verbose) corelib(*xml << "-- Cbal=","", C0);


            callB B = { *this };


            // compute gradient/balance
            Bal.free();
            bool bad = false;
            for(const snode *node=corelib.head();node;node=node->next)
            {
                const species &s = ***node;
                const size_t   j = *s;
                const double   c = Corg[j];
                switch(s.rank)
                {
                    case 0:
                        break;

                    case 1:
                        assert(c>=0);
                        break;

                    default:
                        if(c<0)
                        {
                            Bal << squared(c);
                            dC[j]  = -c;
                            bad    = true;
                        }
                        else
                        {
                            dC[j] = 0;
                        }
                }
            }
            double B0 =  xadd.tableau(Bal)/2;
            std::cerr << "Bal = " << Bal << std::endl;
            std::cerr << "B0  = " << B0   << std::endl;
            std::cerr << "S   = " << dC   << std::endl;

            if(bad)
            {
                std::cerr << singles << std::endl;
                size_t nrun = 0;
                NuA.assign(Nu);
                for(const enode *node=singles.head();node;node=node->next)
                {
                    const equilibrium      &eq = ***node;
                    const size_t            ei = *eq;
                    const components::state es =  eq.state_at(Corg);
                    double                 &xx = xi[ei];
                    if(verbose) singles.pad(std::cerr << eq.name,eq) << " " << eq.state_text(es);

                    switch (es) {
                        case components::are_blocked: {
                            // block whatever
                            NuA[ei].ld(0);
                            blocked[ei] = true;
                            xx          = 0;
                        } break;

                        case components::are_running: {
                            // guess
                            ++nrun;
                            blocked[ei] = false;
                            xx          = xadd.dot(NuA[ei],dC);
                            std::cerr << '@' << std::setw(15) << xx;
                        } break;
                    }

                    if(verbose)  std::cerr << std::endl;
                }
                std::cerr << "xxx =" << xi  << std::endl;
                for(const anode *node=working.head;node;node=node->next)
                {
                    const species &s = **node;
                    const size_t   j = *s;
                    xadd.free();
                    for(size_t i=N;i>0;--i) xadd.push( NuA[i][j] * xi[i] );
                    dC[j] = xadd.get();
                }
                std::cerr << "beta=" << dC << std::endl;



                triplet<double>  u = { 0, -1, 1 };
                triplet<double>  b = { B0, -1, B(u.c) };

                bool             success = false;
                while(true) {

                    if(b.c<=0)
                    {
                        std::cerr << "Success!" << std::endl;
                        b.b = b.c;
                        u.b = u.c;
                        success = true;
                        break;
                    }
                    if(b.c<b.a)
                    {
                        u.c += u.c;
                        b.c  = B(u.c);
                    }
                    else
                    {
                        break;
                    }
                }

                std::cerr << u << " -> " << b << std::endl;

                {
                    ios::ocstream fp("bal.dat");
                    const size_t NP = 100;
                    for(size_t i=0;i<=NP;++i)
                    {
                        const double uu = i/double(NP);
                        fp("%.15g %.15g\n", uu, B(uu) );
                    }
                }

                if(!success)
                {
                    optimize::run_for(B,u,b,optimize::inside);
                    success = (b.b<=0);
                }

                std::cerr << u << " -> " << b << std::endl;

                if(success)
                {
                    std::cerr << "SUCCESS" << std::endl;
                }
                else
                {
                    std::cerr << "No Success" << std::endl;
                }



                exit(0);
            }


            return true;
        }

    }

}

