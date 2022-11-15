#include "yack/chem/reactor.hpp"
#include "yack/ios/xmlog.hpp"
#include "yack/type/utils.hpp"
#include "yack/math/opt/optimize.hpp"
#include "yack/math/iota.hpp"

#include "yack/type/boolean.h"
#include "yack/exception.hpp"
#include "yack/math/numeric.hpp"

#include <iomanip>

namespace yack
{
    using namespace math;

    namespace chemical
    {




        double reactor:: Balance(const double u)
        {
            //static const double cmin = sqrt( numeric<double>::minimum );
            static const double cmin = pow(10.0, ceil(numeric<double>::min_10_exp/2) );

            for(const anode *node=working.head;node;node=node->next)
            {
                const size_t j = ***node;
                Ctry[j] = Cbal[j] + u * dC[j];
            }
            primaryRecover(Ctry);

            for(const anode *node=working.head;node;node=node->next)
            {
                const species &s = **node; if(s.rank<=1) continue;
                const size_t  j  = *s;
                double       &c  = Ctry[j];
                if( fabs(c) <= cmin ) c=0;
            }

            return Balance(Ctry);
        }


        bool reactor:: primaryBalance(const xmlog &xml)
        {
            static const char fn[] = "primaryBalance";
            YACK_XMLSUB(xml,fn);
            bool primaryBalanced = true;
            for(const enode *node=singles.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                YACK_XMLSUB(xml,eq.name);
                if(!eq.try_primary_balance(Cbal,&xml))
                {
                    primaryBalanced = false;
                }
            }
            YACK_XMLOG(xml, "==> " << fn << ' ' << (primaryBalanced?yack_success:yack_failure) << " <==");
            return primaryBalanced;
        }

        void reactor:: primaryRecover(writable<double> &C) const
        {
            for(const enode *node=singles.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                if(!eq.try_primary_balance(C,NULL))
                {
                    throw exception("cannot recover <%s>", eq.name() );
                }
            }


        }



        struct callB
        {
            reactor &self;

            double operator()(double u)
            {
                return self.Balance(u);
            }
        };

        double reactor:: Balance(const readable<double> &C)
        {
            xadd.free();
            for(const anode *node=working.head;node;node=node->next)
            {
                const size_t j = ***node;
                const double c = C[j];
                if(c<0)
                {
                    xadd << (c*c);
                }
            }
            return xadd.get();
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
                    xadd << (c*c);
                    beta[j] = -c;
                    well    = false;
                }
                else
                {
                    beta[j] = 0;
                }
            }

            return well;

        }



        bool reactor:: balance(writable<double> &C0)
        {
            static const char fn[] = "[reactor]";
            const xmlog       xml(fn,std::cerr,entity::verbose);
            YACK_XMLSUB(xml,"Balancing");
            if(verbose) corelib(*xml << "-- Cini=","", C0);


            //------------------------------------------------------------------
            //
            //
            // Check Status
            //
            //
            //------------------------------------------------------------------
            if(N<=0) {
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
                beta[j] = 0;
                dC[j]   = 0;
            }

            if(!primaryBalance(xml)) return false;

            vector<int> xd(N,0);
            //------------------------------------------------------------------
            //
            //
            // initialize balance and directions
            //
            //
            //------------------------------------------------------------------
            {
                xadd.free();
                bool well = true;
                for(const anode *node=working.head;node;node=node->next)
                {
                    const species &s = **node;
                    const size_t   j = *s;
                    const double   c = Cbal[j];
                    if(verbose) corelib.pad(std::cerr << "| [" << s.name << "]",s) << " = " << std::setw(15) << c << ' ';
                    switch(s.rank)
                    {
                        case 0: break;
                        case 1: assert(c>=0);
                            if(c<=0)
                            {
                                if(verbose) std::cerr << "(*)";
                                beta[j] = 1;
                            }
                            else
                            {
                                if(verbose) std::cerr << "(+)";
                                beta[j] = 0;
                            }
                            break;

                        default:
                            if(c<0)
                            {
                                if(verbose) std::cerr << "(-)";
                                beta[j] = 1;
                                well    = false;
                                xadd << -c;
                            }
                            else
                            {
                                if(verbose) std::cerr << "(+)";
                                beta[j] = 0;
                            }
                    }
                    if(verbose) std::cerr << std::endl;

                }

                if(well)
                {
                    YACK_XMLOG(xml,"-- balanced");
                    working.transfer(C0,Cbal);
                    return true;
                }
            }



            const double B0 = xadd.get();
            YACK_XMLOG(xml,"-- B0     = " << B0);
            YACK_XMLOG(xml,"-- beta   = " << beta);

            // computing xd = NuA * beta
            {
                NuA.assign(Nu);
                int xmax = 0;
                for(const enode *node=singles.head();node;node=node->next)
                {
                    const equilibrium & eq = ***node;
                    const size_t        ei = *eq;
                    writable<int>      &nu = NuA[ei];
                    int                &xx = xd[ei];
                    xx = iota::dot<int>::of(nu,beta);

                    if(verbose) singles.pad(std::cerr << "| <" << eq.name << ">",eq) << " = " << std::setw(4) << xx << ": ";

                    switch( __sign::of(xx) )
                    {
                        case __zero__:
                            if(verbose) std::cerr << "unused.";
                            xx = 0;
                            nu.ld(0);
                            break;

                        case positive: {
                            const xlimits &lm = eq.primary_limits(Cbal,0);
                            if(verbose) std::cerr << lm;
                            if(lm.reac && lm.reac->xi<=0)
                            {
                                if(verbose) std::cerr << " [[canceled by " << (***lm.reac).name << "]]";
                                xx = 0;
                                nu.ld(0);
                            }
                            else
                            {
                                xmax = max_of(xmax,xx);
                            }
                        } break;

                        case negative: {
                            const xlimits &lm = eq.primary_limits(Cbal,0);
                            if(verbose) std::cerr << lm;
                            if(lm.prod && lm.prod->xi<=0)
                            {
                                if(verbose) std::cerr << " [[canceled by " << (***lm.prod).name << "]]";
                                xx = 0;
                                nu.ld(0);
                            }
                            else
                            {
                                xmax = max_of(xmax,-xx);
                            }
                        } break;
                    }

                    if(verbose) std::cerr << std::endl;
                }
                YACK_XMLOG(xml,"-- |xmax| = " << xmax);
            }

            // computing Alpha matrix = NuA' * diagm(xd)
            std::cerr << "NuA=" << NuA << std::endl;
            std::cerr << "xd =" << xd  << std::endl;

            imatrix Alpha(N,M);
            for(size_t i=N;i>0;--i)
            {
                const int d = xd[i];
                for(size_t j=M;j>0;--j)
                {
                    Alpha[i][j] = NuA[i][j] * d;
                }
            }

            std::cerr << "Alpha=" << Alpha << std::endl;
            std::cerr << "C    =" << Cbal  << std::endl;

            vector<double> rho(N,0);
            for(size_t i=1;i<=N;++i)
            {
                const readable<int> &alpha = Alpha[i];
                rho.free();
                std::cerr << "using " << alpha << std::endl;
                bool discard = false;
                for(const anode *node=working.head;node;node=node->next)
                {
                    const species &s = **node;
                    const size_t   j = *s;
                    const int      d = alpha[j]; if(!d) continue;;
                    const double   c = Cbal[j];
                    corelib.pad(std::cerr << "\t[" << s.name << "]",s) << " = " << std::setw(15) << c <<" with " << std::setw(15) << d << std::endl;
                    if(d<0)
                    {
                        if(c<0)
                        {
                            std::cerr << "Discarding" << std::endl;
                            discard = true;
                            goto DONE;
                        }
                        else
                        {
                            rho << c/(-d);
                        }
                    }
                    else
                    {

                    }
                }
            DONE:
                if(discard) continue;
                std::cerr << "rho: " << rho << std::endl;
            }


            exit(0);
            return false;
        }


#if 0
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
                beta[j] = 0;
                dC[j]   = 0;

            }

            callB B = { *this };


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
            // compute initial balance and gradient
            //
            //
            //------------------------------------------------------------------
            unsigned cycle = 0;
        CYCLE:
            ++cycle;
            YACK_XMLOG(xml, "-------- cycle #" << cycle << " --------");
            if( isWellBalanced() )
            {
                //--------------------------------------------------------------
                //
                // all done
                //
                //--------------------------------------------------------------
                YACK_XMLOG(xml,"-- balanced");
                working.transfer(C0,Cbal);
                return true;
            }
            else
            {
                //--------------------------------------------------------------
                //
                // initialize
                //
                //--------------------------------------------------------------
                const double B0 = xadd.get();
                assert(B0>0);
                assert(B0>0);
                assert( fabs(B0-B(0))<=0 );

                YACK_XMLOG(xml, "-- B0     = " << B0);
                YACK_XMLOG(xml, "-- beta   = " << beta);
                NuA.assign(Nu);
                {
                    double xmax = 0;
                    for(const enode *node = singles.head(); node; node=node->next)
                    {
                        const equilibrium &eq = ***node;
                        const size_t       ei = *eq;
                        writable<int>     &nu = NuA[ei];
                        double            &xx = xi[ei];

                        xx = working.dot(nu,beta,xadd);
                        if(verbose) singles.pad(std::cerr << "| " << eq.name,eq) << " @" << std::setw(15) << xx << ": ";
                        switch( __sign::of(xx) )
                        {
                            case __zero__:
                                xx = 0;
                                nu.ld(0);
                                std::cerr << "[unused]";
                                break;

                            case positive: {
                                const xlimits &lm = eq.primary_limits(Cbal,corelib.maxlen);
                                if(verbose) std::cerr << lm;
                                if(lm.reac && lm.reac->xi<=0)
                                {
                                    std::cerr << " [ canceled by [" << (***lm.reac).name << "] ]";
                                    xx = 0;
                                    nu.ld(0);
                                }
                                else
                                {
                                    xmax = max_of(xmax,xx);
                                }
                            } break;

                            case negative : {
                                const xlimits &lm = eq.primary_limits(Cbal,corelib.maxlen);
                                if(verbose) std::cerr << lm;
                                if(lm.prod && lm.prod->xi<=0)
                                {
                                    std::cerr << " [ canceled by [" << (***lm.prod).name << "] ]";
                                    xx = 0;
                                    nu.ld(0);
                                }
                                else
                                {
                                    xmax = max_of(xmax,-xx);
                                }
                            } break;
                        }

                        if(verbose) std::cerr << std::endl;
                    }
                    YACK_XMLOG(xml, "-- |xmax| = " << xmax);
                    YACK_XMLOG(xml, "-- xi_dir = " << xi);
                }

                // computing dC
                for(const anode *node=working.head;node;node=node->next)
                {
                    const species &s = **node;
                    const size_t   j = *s;

                    xadd.free();
                    for(size_t i=N;i>0;--i)
                    {
                        xadd << (NuA[i][j] * xi[i] );
                    }
                    dC[j] = xadd.get();
                    if(verbose) {
                        corelib.pad(*xml << '[' << s.name << ']',s) << '=' << std::setw(15) << Cbal[j];
                        std::cerr << " + alpha * (" << std::setw(15) << dC[j] << ")";
                        std::cerr << std::endl;
                    }



                }

                const double scaling = working.dot(beta,dC,xadd);
                std::cerr << "scaling=" << scaling << std::endl;

                triplet<double> u       = { 0,  -1, B0/scaling };
                triplet<double> f       = { B0, -1, B(u.c)    };
                bool            success = false;
                while(true)
                {
                    if(f.c<=0)
                    {
                        // set consistent for shrinking
                        f.b = f.c = 0;
                        u.b = u.c;
                        success = true;
                        break;
                    }

                    if(f.c>=f.a)
                    {
                        break;
                    }

                    f.c = B( u.c += u.c );
                }

                YACK_XMLOG(xml,u << " -> " << f);

                if(false)
                {
                    ios::ocstream fp("bal.dat"); std::cerr << "\t\tSaving BAL" << std::endl;
                    const size_t np = 1000;
                    for(size_t i=0;i<=np;++i)
                    {
                        const double uu = (u.c*i)/double(np);
                        fp("%.15g %.15g\n", uu, B(uu) );
                    }
                }

                if(!success)
                {
                    YACK_XMLOG(xml, "backtracking");
                    optimize::run_for(B, u, f, optimize::inside);
                    // set consistent for shrinking
                    f.a = B0;  u.a = 0;
                    f.c = f.b; u.c = u.b;
                    success = (f.b<=0);
                    YACK_XMLOG(xml,u << " -> " << f);
                }



                if(success)
                {
                    YACK_XMLOG(xml, "-- success @" << u.b);
                    working.transfer(Cend,Ctry);
                    assert( Balance(Cend) <= 0);
                    while(true)
                    {
                        u.b = clamp(u.a,0.5*(u.a+u.c),u.c);
                        f.b = B(u.b);
                        if(f.b<=0)
                        {
                            u.c = u.b;
                            f.c = 0;
                            working.transfer(Cend,Ctry);
                        }
                        else
                        {
                            u.a = u.b;
                            f.a = f.b;
                        }

                        const double wlim = 1e-2* fabs(u.b);
                        const double wnow = fabs(u.c-u.a);
                        if(wnow<=wlim)
                        {
                            break;
                        }
                    }
                    YACK_XMLOG(xml,u << " -> " << f);
                    {
                        ios::acstream fp(track);
                        fp("%u",cycle);
                        corelib.frame(fp,Cend) << '\n';
                    }
                    working.transfer(C0,Cend);
                    return true;
                }
                else
                {
                    {
                        ios::acstream fp(track);
                        fp("%u",cycle);
                        corelib.frame(fp,Ctry) << '\n';
                    }
                    const double B1 = f.b; assert(B1>0); assert( fabs(B1-Balance(Ctry)) <= 0 );
                    if(B1<B0)
                    {
                        working.transfer(Cbal,Ctry);
                        goto CYCLE;
                    }

                    std::cerr << "Stalled" << std::endl;
                    exit(0);
                    return false;
                }
            }


        }
#endif


    }

}


