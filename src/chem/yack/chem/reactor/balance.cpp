#include "yack/chem/reactor.hpp"
#include "yack/ios/xmlog.hpp"
#include "yack/type/utils.hpp"
#include "yack/math/opt/optimize.hpp"
#include "yack/math/iota.hpp"
#include <iomanip>
#include <cfloat>
#include "yack/type/boolean.h"
#include "yack/exception.hpp"
#include "yack/container/tuple/pair.hpp"

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
                    xadd << squared( dC[j] = -c);
                }
                else
                {
                    dC[j] = 0;
                }
            }

            return well;

#if 0
            for(const snode *node=corelib.head();node;node=node->next)
            {
                const species &s = ***node;
                const size_t   j = *s;
                const double   c = Cbal[j];
                if(s.rank>0)
                {
                    const sign_type cs = __sign::of(c);
                    if(verbose)
                    {
                        corelib.pad(std::cerr << '[' << s.name << ']',s) << " = " << std::setw(15) << c;
                    }
                    switch(cs)
                    {
                        case __zero__:
                            beta[j] = 1;
                            if(verbose) std::cerr << " (*)";
                            break;

                        case negative:
                            xadd << -c;
                            well = false;
                            beta[j] = 1;
                            if(verbose) std::cerr << " (-)";
                            break;

                        case positive:
                            beta[j] = 0;
                            if(verbose) std::cerr << " (+)";
                            break;
                    }
                    if(verbose) std::cerr << std::endl;
                }

            }
            return well;
#endif

        }


        struct callB
        {
            reactor &self;

            double operator()(double u)
            {
                return self.Balance(u);
            }
        };


        

        YACK_PAIR_DECL(STANDARD,Cut,double,f,const species *,s);

        static inline int Compare(const Cut &lhs, const Cut &rhs) throw()
        {
            return comparison::increasing(lhs.f,rhs.f);
        }
        

        YACK_PAIR_END(Cut);



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
                beta[j] = 0;
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

            if( isWellBalanced() )
            {
                YACK_XMLOG(xml,"-- well balanced");
                working.transfer(C0,Cbal);
                return true;
            }
            else
            {
                callB B = { *this };
                const double B0 = xadd.get();
                assert( fabs(B0-B(0))<=0 );
                if(verbose) std::cerr << singles << std::endl;
                YACK_XMLOG(xml,"B0   = " << B0);
                YACK_XMLOG(xml,"beta = " << dC);

                NuA.assign(Nu);
                double xmax = 0;
                for(const enode *node=singles.head();node;node=node->next)
                {
                    const equilibrium   &eq = ***node;
                    const size_t         ei = *eq;
                    double              &xx = xi[ei];
                    const readable<int> &nu = Nu[ei];

                    xx = xadd.dot(nu,dC);
                    if(verbose) singles.pad(std::cerr << eq.name,eq) << " @" << std::setw(15) << xx << ' ';

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

                std::cerr << "|xmax|=" << xmax << std::endl;
                singles(std::cerr,"d_",xi);
                singles(std::cerr << "NuA=","",NuA);

                // computing direction of C
                vector<Cut> cutting;
                for(const anode *node=working.head;node;node=node->next)
                {
                    const species &s = **node;
                    const size_t   j = *s;

                    xadd.free();
                    for(size_t i=N;i>0;--i) xadd << (NuA[i][j] * xi[i]);
                    const double d = dC[j] = xadd.get();
                    if(d<0 && 1==s.rank)
                    {
                        const double c = Cbal[j]; assert(c>=0);
                        const double f = c/(-d);
                        const Cut    cut(f,(species *)&s);
                        cutting << cut;
                        if(verbose) YACK_XMLOG(xml,"limiting by " << s.name << " @" << f);
                    }
                }
                corelib(std::cerr,"d_",dC);
                std::cerr << "cutting=" << cutting << std::endl;

#if 0
                const double dC2 = xadd.squares(dC);
                std::cerr << "B0    =" << B0  << std::endl;
                std::cerr << "|dC|^2=" << dC2 << std::endl;
                std::cerr << "cutting=" << cutting << std::endl;

                const double    alpha =  (B0/dC2);
                triplet<double> u     = { 0,  -1, alpha };
                triplet<double> F     = { B0, -1, -1 };
                if(cutting.size())
                {
                    hsort(cutting,comparison::increasing<double>);
                    const double umax = cutting.front()/2;

                    u.c = umax;
                    F.c = B(u.c);
                }
                else
                {
                    // no limits!!
                    while( (F.c = B(u.c)) < F.a )
                    {
                        u.c += u.c;
                    }
                }

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

                optimize::run_for(B,u,F,optimize::inside);

                std::cerr << "B(" << u.b << ")=" << F.b << std::endl;
#endif


                exit(0);
                return false;
            }

        }

#if 0
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
                beta[j] = 0;
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


            const bool  well = isWellBalanced();
            vector<int> d_xi(N,0);

            if(well)
            {
                YACK_XMLOG(xml,"-- well balanced");
                working.transfer(C0,Cbal);
                return true;
            }
            else
            {
                callB B = { *this };
                const double B0 = xadd.get();
                assert( fabs(B0-B(0))<=0 );
                if(verbose) std::cerr << singles << std::endl;
                YACK_XMLOG(xml,"B0   = " << B0);
                YACK_XMLOG(xml,"beta = " << beta);

                // computing xi direction
                NuA.assign(Nu);
                iota::mul(d_xi,Nu,beta);

                // cutting
                {
                    int xmax = 0;
                    for(const enode *node=singles.head();node;node=node->next)
                    {
                        const equilibrium &eq = ***node;
                        const size_t       ei = *eq;
                        int               &xx = d_xi[ei];
                        if(verbose)  singles.pad(std::cerr << "-- " << eq.name,eq) << " @" << std::setw(3) << xx << " : ";


                        if(xx>0)
                        {
                            const xlimits     &lm = eq.primary_limits(Cbal,corelib.maxlen);
                            if(verbose) std::cerr << lm;
                            if(lm.reac && lm.reac->xi<=0)
                            {
                                if(verbose) std::cerr << " <limited by " << (***lm.reac).name << ">";
                                xx = 0;
                                NuA[ei].ld(0);
                            }
                        }
                        else
                        {
                            if(xx<0)
                            {
                                const xlimits     &lm = eq.primary_limits(Cbal,corelib.maxlen);
                                if(verbose) std::cerr << lm;
                                if(lm.prod && lm.prod->xi<=0)
                                {
                                    if(verbose) std::cerr << " <limited by " << (***lm.prod).name << ">";
                                    xx = 0;
                                    NuA[ei].ld(0);
                                }
                            }
                            else
                            {
                                // not used...
                                NuA[ei].ld(0);
                                std::cerr << "<unused>";
                            }
                        }

                        xmax = max_of(xmax,absolute(xx));
                        if(verbose) std::cerr << std::endl;
                    }

                    YACK_XMLOG(xml,"|xmax| = " << xmax);
                    if(xmax<=0)
                    {
                        YACK_XMLOG(xml,"-- stalled");
                        return false;
                    }
                }


                if(verbose) singles(std::cerr,"dxi_",d_xi);


                vector<double> cutting(M,as_capacity);

                xadd.free();
                double dmax = 0;
                for(const anode *node=working.head;node;node=node->next)
                {
                    const species &s = **node;
                    const size_t   j = *s;

                    int tmp = 0;
                    for(size_t i=N;i>0;--i)
                    {
                        tmp += NuA[i][j] * d_xi[i];
                    }
                    dmax = max_of(dmax, fabs(dC[j]=tmp) );
                    if(verbose) corelib.pad( *xml << "dir_[" << s.name << "]",s) << " = " << std::setw(4) << tmp;
                    if(tmp<0 && 1==s.rank)
                    {
                        const double c = Cbal[j]; assert(c>=0);
                        const double f = c/(-tmp);
                        cutting << f;
                        if(verbose) std::cerr << " cutting@" << std::setw(15) << f;
                    }
                    if(verbose) std::cerr << std::endl;
                }
                hsort(cutting,comparison::increasing<double>);
                if(verbose)
                {

                    std::cerr << "dmax    = " << dmax    << std::endl;
                    std::cerr << "cutting = " << cutting << std::endl;
                }

                triplet<double> u = { 0, -1, B0 };
                triplet<double> f = { B0, -1, -1 };
                if(cutting.size())
                {
                    u.c = min_of(u.c,cutting.front());
                    f.c = B(u.c);
                    std::cerr << "u: " << u << " -> " << f << std::endl;

                }
                else
                {

                    exit(0);
                }

                {
                    ios::ocstream fp("bal.dat");
                    const size_t NP = 100;
                    for(size_t i=0;i<=NP;++i)
                    {
                        const double uu = (i/double(NP)) * u.c;
                        fp("%1.5g %.15g\n", uu, B(uu));
                    }

                }

                exit(0);



                exit(0);

                return false;
            }

        }
#endif



    }

}

