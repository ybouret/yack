#include "yack/chem/reactor.hpp"
#include "yack/ios/xmlog.hpp"
#include "yack/type/utils.hpp"
#include "yack/math/opt/optimize.hpp"
#include "yack/math/iota.hpp"
#include <iomanip>
#include <cfloat>
#include "yack/type/boolean.h"

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
                if(c<0) Bal << max_of<double>(squared(c),DBL_MIN);
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


        bool reactor:: primaryBalance(const xmlog &xml)
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
            YACK_XMLOG(xml, " ---- " << (primaryBalanced?yack_success:yack_failure) << " ----");
            return primaryBalanced;
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

            //------------------------------------------------------------------
            //
            // initialize phase space
            //
            //------------------------------------------------------------------
            for(size_t j=M;j>0;--j)
            {
                Corg[j] = Ctry[j] = C0[j];
                dC[j]   = 0;
            }

            //------------------------------------------------------------------
            //
            // ensure first primary balance
            //
            //------------------------------------------------------------------
            if(!primaryBalance(xml)) return false;
            if(verbose) corelib(*xml << "-- Cbal=","", Corg);

            
            callB B = { *this };


            unsigned cycle = 0;
        CYCLE:
            ++cycle;
            YACK_XMLOG(xml, "---- balancing cycle #" << cycle << " ----");

            //------------------------------------------------------------------
            //
            // compute both gradient in dC and balance
            //
            //------------------------------------------------------------------
            Bal.free();
            bool bad = false;
            for(const snode *node=corelib.head();node;node=node->next)
            {
                const species &s = ***node;
                const size_t   j = *s;
                const double   c = Corg[j];
                switch(s.rank)
                {
                    case 0:               break;
                    case 1: assert(c>=0); break;
                    default:
                        if(c<0)
                        {
                            Bal << max_of<double>(squared(c),DBL_MIN);
                            dC[j]  = -c;
                            bad    = true;
                        }
                        else
                        {
                            dC[j] = 0;
                        }
                }
            }

            if(!bad)
            {
                YACK_XMLOG(xml,"-- fully balanced");
                working.transfer(C0,Corg);
                return true;
            }


            double B0 =  xadd.tableau(Bal)/2;
            std::cerr << "B0   = " << B0   << std::endl;
            std::cerr << "beta = " << dC   << std::endl;

            assert(fabs(B0-B(0)) <=0  );

            //------------------------------------------------------------------
            //
            // preparing xi DIRECTIONS in xi
            //
            //------------------------------------------------------------------
            NuA.assign(Nu);
            size_t nrun = 0;
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
                        blocked[ei] = false;
                        xx          = xadd.dot(NuA[ei],dC);
                        if(verbose) std::cerr << '@' << std::setw(15) << xx;

                        switch( __sign::of(xx) )
                        {
                            case __zero__:
                                ++nrun;
                                xx = 0;
                                if(verbose) std::cerr << " [accept0] ";
                                break;

                            case positive: {
                                const xlimits &lm = eq.primary_limits(Corg,corelib.maxlen);
                                if( (limited_by_both==lm.type || limited_by_reac == lm.type) && lm.reac->xi <= 0)
                                {
                                    if(verbose) std::cerr << " [reject+] ";
                                    xx = 0;
                                    blocked[ei] = true;
                                    NuA[ei].ld(0);
                                }
                                else
                                {
                                    if(verbose) std::cerr << " [accept+] ";
                                    ++nrun;
                                }
                                if(verbose) std::cerr << lm;
                            } break;


                            case negative : {
                                const xlimits &lm = eq.primary_limits(Corg,corelib.maxlen);
                                if( (limited_by_both==lm.type || limited_by_prod == lm.type) && lm.prod->xi <= 0)
                                {
                                    if(verbose) std::cerr << " [reject-] ";
                                    xx = 0;
                                    blocked[ei] = true;
                                    NuA[ei].ld(0);
                                }
                                else
                                {
                                    if(verbose) std::cerr << " [accept-] ";
                                    ++nrun;
                                }
                                if(verbose) std::cerr << lm;
                            } break;
                        }


                    } break;
                }

                if(verbose)  std::cerr << std::endl;
            }

            if(verbose) singles(std::cerr << "xi_dir=","",xi);

            //------------------------------------------------------------------
            //
            // computing directions
            //
            //------------------------------------------------------------------
            for(const anode *node=working.head;node;node=node->next)
            {
                const species &s = **node;
                const size_t   j =  *s;

                xadd.free();
                for(size_t i=N;i>0;--i)
                {
                    xadd.push( NuA[i][j] * xi[i] );
                }
                dC[j] = xadd.get();
            }


            double u1 = 1;
            double B1 = B(1);
            if(verbose)
            {
                corelib(std::cerr << "dC_dir=","",dC);
                std::cerr << "B(0)=" << B0 << std::endl;
                std::cerr << "B(" << u1 << ")=" << B1 << std::endl;
            }


            bool success = false;
            while(true)
            {
                if(B1<=0)
                {
                    success = true;
                    break;
                }

                if(B1>=B0)
                {
                    break;
                }

                B1 = B( u1 += u1 );
                if(verbose) std::cerr << "B(" << u1 << ")=" << B1 << std::endl;
            }


            {
                ios::ocstream fp("bal.dat");
                const size_t NP = 1000;
                for(size_t i=0;i<=NP;++i)
                {
                    const double uu = u1 * (i/double(NP));
                    fp("%.15g %.15g\n", uu, B(uu));
                }
            }


            if(!success)
            {
                triplet<double> u = { 0,  -1, u1 };
                triplet<double> b = { B0, -1, B1 };
                optimize::run_for(B,u,b,optimize::inside);
                u1 = u.b;
                B1 = b.b;
                success = (B1<=0);
                if(verbose)
                {
                    std::cerr << "B(" << u1 << ")=" << B1 << std::endl;
                    std::cerr << "success=" << yack_boolean(success) << std::endl;
                }
            }

            if(success)
            {
                working.transfer(C0,Ctry);
                if(verbose) corelib(*xml << "-- Cbal=","", C0);
                return true;
            }
            else
            {
                if(B1>=B0)
                {
                    YACK_XMLOG(xml, "-- invalid negative concentrations");
                    exit(0);
                    return false;
                }
                else
                {
                    working.transfer(Corg,Ctry);
                    if(!primaryBalance(xml))
                    {
                        YACK_XMLOG(xml, "-- unexpected failure in primary re-balancing");
                        return false;
                    }

                    goto CYCLE;
                }
            }

        }

    }

}

