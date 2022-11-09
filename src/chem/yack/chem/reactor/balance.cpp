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
                if(c<0) Bal <<  squared(c);
            }
            return xadd.tableau(Bal)/2;
        }

        double reactor:: Balance(const double u)
        {
            for(const anode *node=working.head;node;node=node->next)
            {
                const size_t j = ***node;
                Ctry[j] = Cbal[j] + u * dC[j];
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
                if(eq.try_primary_balance(Cbal,xml))
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

        bool reactor:: isWellBalanced()
        {
            Bal.free();
            bool well = true;
            for(const snode *node=corelib.head();node;node=node->next)
            {
                const species &s = ***node;
                const size_t   j = *s;
                const double   c = Cbal[j];
                switch(s.rank)
                {
                    case 0:               break;
                    case 1: assert(c>=0); break;
                    default:
                        if(c<0)
                        {
                            Bal << squared(c);
                            dC[j]  = -c;
                            well   = false;
                        }
                        else
                        {
                            dC[j] = 0;
                        }
                }
            }

            return well;
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
            //
            // initialize phase space
            //
            //
            //------------------------------------------------------------------
            for(size_t j=M;j>0;--j)
            {
                Cbal[j] = Ctry[j] = C0[j];
                dC[j]   = 0;
            }

            //------------------------------------------------------------------
            //
            //
            // ensure first primary balance
            //
            //
            //------------------------------------------------------------------
            if(!primaryBalance(xml)) return false;
            if(verbose) corelib(*xml << "-- Cbal=","", Cbal);

            
            callB B = { *this };


            unsigned cycle = 0;
        CYCLE:
            ++cycle;
            YACK_XMLOG(xml, "---- balancing cycle #" << cycle << " ----");

            //------------------------------------------------------------------
            //
            //
            // compute both gradient in dC and balance
            //
            //
            //------------------------------------------------------------------

            // load Bal and check for negative active species
            const bool well = isWellBalanced();

            if(well)
            {
                YACK_XMLOG(xml,"-- fully balanced @cycle #" << cycle);
                working.transfer(C0,Cbal);
                return true;
            }

            // deduce B0
            double B0 =  xadd.tableau(Bal)/2;
            assert(fabs(B0-B(0)) <=0  );

            //------------------------------------------------------------------
            //
            //
            // preparing xi DIRECTIONS in xi
            //
            //
            //------------------------------------------------------------------
            NuA.ld(0); xi.ld(0); blocked.ld(true);
            size_t nrun = 0;
            for(const enode *node=singles.head();node;node=node->next)
            {
                //--------------------------------------------------------------
                //
                // get equilibrium and its state
                //
                //--------------------------------------------------------------
                const equilibrium      &eq = ***node;
                const size_t            ei = *eq;
                const components::state es =  eq.state_at(Cbal);
                if(verbose) singles.pad(std::cerr << eq.name,eq) << " " << eq.state_text(es);

                switch (es) {
                    case components::are_blocked: {
                        //------------------------------------------------------
                        //
                        // declare blocked, left untouched
                        //
                        //------------------------------------------------------
                    } break;

                    case components::are_running: {
                        //------------------------------------------------------
                        //
                        // guess
                        //
                        //------------------------------------------------------
                        double                 &xx = xi[ei];
                        const readable<int>    &nu = Nu[ei];
                        xx = xadd.dot(nu,dC);
                        if(verbose) std::cerr << '@' << std::setw(15) << xx;

                        switch( __sign::of(xx) )
                        {
                            case __zero__:
                                if(verbose) std::cerr << " [accept0] ";
                                xx = 0; // avoid rouding errors
                                ++nrun; blocked[ei] = false; iota::load(NuA[ei],nu);
                                break;

                            case positive: {
                                const xlimits &lm = eq.primary_limits(Cbal,corelib.maxlen);
                                if( (limited_by_both==lm.type || limited_by_reac == lm.type) && lm.reac->xi <= 0)
                                {
                                    if(verbose) std::cerr << " [reject+] ";
                                    xx = 0;
                                }
                                else
                                {
                                    if(verbose) std::cerr << " [accept+] ";
                                    ++nrun; blocked[ei] = false; iota::load(NuA[ei],nu);
                                }
                                if(verbose) std::cerr << lm;
                            } break;


                            case negative : {
                                const xlimits &lm = eq.primary_limits(Cbal,corelib.maxlen);
                                if( (limited_by_both==lm.type || limited_by_prod == lm.type) && lm.prod->xi <= 0)
                                {
                                    if(verbose) std::cerr << " [reject-] ";
                                    xx = 0;
                                }
                                else
                                {
                                    if(verbose) std::cerr << " [accept-] ";
                                    ++nrun; blocked[ei] = false; iota::load(NuA[ei],nu);
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
            // computing concentrations directions
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


            //------------------------------------------------------------------
            //
            // initial look-up
            //
            //------------------------------------------------------------------
            double u1 = 1;
            double B1 = B(1);
            if(verbose)
            {
                corelib(std::cerr << "dC_dir=","",dC);
                std::cerr << "B(0)=" << B0 << std::endl;
                std::cerr << "B(" << u1 << ")=" << B1 << std::endl;
            }


            //------------------------------------------------------------------
            //
            // check balanced or go futer until increased
            //
            //------------------------------------------------------------------
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

            if(false)
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
                //--------------------------------------------------------------
                //
                // try again optimize inside 0:u1
                //
                //--------------------------------------------------------------
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
                //--------------------------------------------------------------
                //
                YACK_XMLOG(xml, "-- shrinking");
                //
                //--------------------------------------------------------------
                triplet<double> x = { 0,  -1, u1 };
                triplet<double> f = { B0, -1, 0  };
                working.transfer(Cend,Ctry);

                if(verbose) corelib(*xml << "-- Cend@1=","", Cend);

                do
                {
                    f.b = B( x.b = clamp(x.a,0.5*(x.a+x.c),x.c) );
                    if(f.b<=0)
                    {
                        f.c = f.b;
                        x.c = x.b;
                        working.transfer(Cend,Ctry);
                    }
                    else
                    {
                        f.a = f.b;
                        x.a = x.b;
                    }
                } while(fabs(x.c-x.a)>=0.01);


                working.transfer(C0,Cend);
                if(verbose) corelib(*xml << "-- Cbal@" << x.b << "=","", C0);
                return true;
            }
            else
            {
                //--------------------------------------------------------------
                //
                // not converged
                //
                //--------------------------------------------------------------
                if(B1>=B0)
                {
                    //----------------------------------------------------------
                    // bad
                    //----------------------------------------------------------
                    YACK_XMLOG(xml, "-- couldn't balance");
                    return false;
                }
                else
                {
                    //----------------------------------------------------------
                    // good
                    //----------------------------------------------------------
                    working.transfer(Cbal,Ctry);
                    if(!primaryBalance(xml))
                    {
                        YACK_XMLOG(xml, "-- unexpected failure in primary re-balancing");
                        return false;
                    }
                    if(verbose) corelib(*xml << "-- Cbal = ","", Cbal);
                    goto CYCLE;
                }
            }

        }

    }

}

