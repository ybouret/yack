#include "yack/chem/reactor.hpp"
#include "yack/ios/xmlog.hpp"
#include "yack/type/utils.hpp"
#include "yack/math/opt/optimize.hpp"
#include "yack/math/iota.hpp"
#include <iomanip>
#include <cfloat>
#include "yack/type/boolean.h"
#include "yack/exception.hpp"
#include "yack/math/numeric.hpp"

namespace yack
{
    using namespace math;

    namespace chemical
    {

        double reactor:: Balance(const readable<double> &C)
        {
            xadd.free();
            for(const anode *node=working.head;node;node=node->next)
            {
                const size_t j = ***node;
                const double c = C[j];
                if(c<0) xadd <<  squared(c);
            }
            return xadd.get();
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
                YACK_XMLSUB(xml,eq.name);
                if(!eq.try_primary_balance(Cbal,xml))
                {
                    primaryBalanced = false;
                }
            }
            YACK_XMLOG(xml, "==> balancing " << (primaryBalanced?yack_success:yack_failure) << " <==");
            return primaryBalanced;
        }

        bool reactor:: isWellBalanced()
        {
            xadd.free();
            bool well = true;
            for(const anode *node=working.head;node;node=node->next)
            {
                const species &s = **node; if(s.rank<=1) continue;
                const size_t   j = *s;
                const double   c = Cbal[j];
                if(c<0)
                {
                    well = false;
                    xadd << squared( beta[j] = -c);
                }
                else
                {
                    beta[j] = 0;
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

            static const char track[] = "track.dat";

            {
                ios::ocstream fp(track);
                corelib.frame(fp << "0",C0) << '\n';
            }
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
                beta[j] = dC[j]   = 0;

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



            //------------------------------------------------------------------
            //
            //
            // compute balance and gradient
            //
            //
            //------------------------------------------------------------------
            unsigned cycle=0;
        CYCLE:
            ++cycle;
            YACK_XMLOG(xml, "-------- cycle #" << cycle << " --------");
            if( isWellBalanced() )
            {
                //--------------------------------------------------------------
                // all done
                //--------------------------------------------------------------
                YACK_XMLOG(xml,"-- balanced");
                working.transfer(C0,Cbal);
                return true;
            }
            else
            {
                //--------------------------------------------------------------
                //
                // get current balance
                //
                //--------------------------------------------------------------
                callB B = { *this };
                const double B0 = xadd.get();
                assert( fabs(B0-B(0))<=0 );
                if(verbose) std::cerr << singles << std::endl;
                YACK_XMLOG(xml,"-- B0     = " << B0);
                YACK_XMLOG(xml,"-- beta   = " << beta);

                //--------------------------------------------------------------
                //
                // computing/cutting direction extents
                //
                //--------------------------------------------------------------
                NuA.assign(Nu);
                {
                    double xmax = 0;
                    for(const enode *node=singles.head();node;node=node->next)
                    {
                        const equilibrium   & eq = ***node;
                        const size_t          ei = *eq;
                        double              & xx = xi[ei];
                        const readable<int> & nu = Nu[ei];

                        xx = working.dot(nu,beta,xadd);
                        if(verbose) singles.pad(std::cerr << "| " << eq.name,eq) << " @" << std::setw(15) << xx << ' ';

                        if(xx>0)
                        {
                            const xlimits &lm = eq.primary_limits(Cbal,corelib.maxlen);
                            if(verbose) std::cerr << lm;
                            if(lm.reac && lm.reac->xi<=0)
                            {
                                if(verbose) std::cerr << " <cancelled by [" << (***lm.reac).name << "]>";
                                xx      = 0;
                                NuA[ei].ld(0);
                            }
                        }
                        else
                        {
                            if(xx<0)
                            {
                                const xlimits &lm = eq.primary_limits(Cbal,corelib.maxlen);
                                if(verbose) std::cerr << lm;
                                if(lm.prod && lm.prod->xi<=0)
                                {
                                    if(verbose) std::cerr << " <cancelled by [" << (***lm.prod).name << "]>";
                                    xx      = 0;
                                    NuA[ei].ld(0);
                                }
                            }
                            else
                            {
                                if(verbose) std::cerr << "<unused>";
                                NuA[ei].ld(0);
                            }
                        }

                        xmax = max_of(xmax,absolute(xx));

                        if(verbose) std::cerr << std::endl;
                    }

                    YACK_XMLOG(xml,"-- |xmax| = " << xmax);
                    YACK_XMLOG(xml,"-- dir_xi = " << xi);
                    YACK_XMLOG(xml,"-- NuA    = " << NuA);
                    YACK_XMLOG(xml,"-- Nu     = " << Nu);

                    if(xmax<=0)
                    {
                        YACK_XMLOG(xml, "-- cancelled extent!!");
                        return false;
                    }
                }

                //--------------------------------------------------------------
                //
                // computing direction of C
                //
                //--------------------------------------------------------------
                {
                    double dmax = 0;
                    for(const anode *node=working.head;node;node=node->next)
                    {
                        const species &s = **node;
                        const size_t   j = *s;
                        xadd.free();
                        for(size_t i=N;i>0;--i) xadd.push(NuA[i][j] * xi[i]);
                        dmax = max_of(dmax, fabs( dC[j] = xadd.get() ) );
                    }
                    YACK_XMLOG(xml,"-- |dmax| = " << dmax);
                    if(dmax<=0)
                    {
                        YACK_XMLOG(xml, "-- cancelled step!!");
                        return false;
                    }
                    if(verbose)
                    {
                        corelib(std::cerr,"",Cbal);
                        corelib(std::cerr,"d_",dC);
                    }
                }

                //--------------------------------------------------------------
                //
                // line search
                //
                //--------------------------------------------------------------
                const double scaling = working.dot(dC,beta,xadd);
                if(scaling<=0)
                {
                    YACK_XMLOG(xml, "-- spurious step!!");
                    return false;
                }
                const double alpha   =   B0/scaling;
                std::cerr << "B0      =" << B0      << std::endl;
                std::cerr << "scaling =" << scaling << std::endl;
                std::cerr << "alpha   =" << alpha   << std::endl;
                triplet<double> u       = { 0,  -1, alpha  };
                triplet<double> F       = { B0, -1, B(u.c) };
                bool            success = false;
                while(true) {
                    success = (F.c<=0);
                    //std::cerr << u << " --> " << F << std::endl;
                    if(success)
                    {
                        u.b = u.c;
                        F.b = F.c;
                        break;
                    }

                    if(F.c>=F.a) break;
                    F.c = B( u.c += u.c );
                }

                YACK_XMLOG(xml,"-- success@level-1=" << yack_boolean(success) << " @" << u.c);

                if(false)
                {
                    std::cerr << "\t\tSAVING BAL" << std::endl;
                    ios::ocstream fp("bal.dat");
                    const size_t NP = 100;
                    for(size_t i=0;i<=NP;++i)
                    {
                        const double uu = (i/double(NP)) * u.c;
                        fp("%1.5g %.15g\n", uu, B(uu));
                    }

                }

                if(!success)
                {
                    // local optimization
                    optimize::run_for(B,u,F,optimize::inside);
                    //std::cerr << u << " --> " << F << std::endl;
                    success = (F.b<=0);
                    YACK_XMLOG(xml,"-- success@level-2=" << yack_boolean(success));
                }

                //--------------------------------------------------------------
                //
                // studying status
                //
                //--------------------------------------------------------------
                if( success )
                {
                    YACK_XMLOG(xml, "-- success: shrinking");
                    // initialize consistent state
                    u.a = 0;   F.a = B0;
                    u.c = u.b; F.c = F.b = 0; working.transfer(Cend,Ctry);

                    assert( Balance(Cend) <= 0 );
                    assert( B(u.c) <= 0);

                    while(true) {
                        u.b = clamp(u.a,0.5*(u.a+u.c),u.c);
                        F.b = B(u.b);
                        if(F.b<=0)
                        {
                            u.c = u.b;
                            F.c = F.b;
                            working.transfer(Cend,Ctry);
                        }
                        else
                        {
                            u.a = u.b;
                            F.a = F.b;
                        }
                        const double wlim = 1e-2 * fabs(u.b);
                        const double wcur = fabs(u.c-u.a);
                        if(wcur<=wlim)
                        {
                            YACK_XMLOG(xml, "-- " << u << " -> " << F);
                            break;
                        }
                    }


                    working.transfer(C0,Cend);
                    return true;
                }
                else
                {
                    working.transfer(Cbal,Ctry);
                    if(!primaryBalance(xml))
                    {
                        YACK_XMLOG(xml, "-- local balance failure");
                        return false;
                    }



                    const double B1 = Balance(Cbal);
                    {
                        ios::acstream fp(track);
                        fp("%u",cycle);
                        corelib.frame(fp,Cbal) << '\n';
                    }
                    YACK_XMLOG(xml, "-- Balance: " << B0 << " --> " << B1);
                    if(B1<B0)
                    {
                        goto CYCLE;
                    }
                    else
                    {
                        std::cerr << "stalled @cycle #" << cycle << std::endl;
                        exit(0);
                    }

                }

            }


        }





    }

}

