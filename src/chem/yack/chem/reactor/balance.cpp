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
                    xadd << -c;
                }
            }
            return xadd.get();
        }

        class agent
        {
        public:
            agent(const double _f, const species & _s) throw() :
            f(_f),
            s(_s)
            {
            }

            agent(const agent &_) throw() : f(_.f), s(_.s) {}

            ~agent() throw()
            {
            }

            inline friend std::ostream & operator<<(std::ostream &os, const agent &a)
            {
                os << '\t' <<  '(' << '[' << a.s.name << ']' <<  '@' << a.f << ')';
                return os;
            }

            static int compare(const agent &lhs, const agent &rhs) throw()
            {
                return comparison::increasing(lhs.f,rhs.f);
            }

            const double    f;
            const species & s;

        private:
            YACK_DISABLE_ASSIGN(agent);
        };

        
        double reactor:: gain(const readable<int> &lam,
                              double              &cf,
                              const species *     &sz)
        {
            assert(NULL==sz);
            assert(fabs(cf)<=0);

            vector<agent> rho(M,as_capacity);
            bool          discard = false;
            for(const anode *node=working.head;node;node=node->next)
            {
                const species &s = **node;
                const size_t   j = *s;
                const int      d = lam[j]; if(!d) continue;
                const double   c = Cbal[j];
                if(verbose) corelib.pad(std::cerr << "\t[" << s.name << "]",s) << " = " << std::setw(15) << c << " with " << std::setw(4) << d << ' ';

                if(0<d)
                {
                    if(c<0)
                    {
                        // increase c<0
                        const agent _( (-c)/d, s);
                        rho << _;
                        if(verbose) std::cerr << "[increase] " << _.f;

                    }
                    else
                    {
                        // increase c>=0
                        if(verbose) std::cerr << "[produced]";
                    }
                }
                else
                {
                    assert(d<0);
                    if(c<=0)
                    {
                        // decrease c<=0 : bad!
                        if(verbose)
                        {
                            discard = true; // postpone return
                            std::cerr << "[discard!]";
                        }
                        else
                        {
                            return -1;
                        }
                    }
                    else
                    {
                        // decrease c>0
                        const agent _( c/(-d), s);
                        rho << _;
                        if(verbose) std::cerr << "[decrease] " << _.f;
                    }
                }


                if(verbose) std::cerr << std::endl;
            }

            if(discard) { assert(verbose); return -1; }
            const size_t na = rho.size();
            if(na<=0)
            {

                return 0;
            }
            else
            {
                hsort(rho,agent::compare);
                const agent &a = rho.front();
                std::cerr << a << std::endl;



                xadd.free();
                for(const anode *node=working.head;node;node=node->next)
                {
                    const species &s = **node;
                    const size_t   j = *s;
                    const int      d = lam[j];
                    const double   c = Cbal[j];
                    Ctry[j] = c + d * a.f;
                    if(d && c<0) xadd << -c;
                }
                Ctry[ *a.s ] = 0;
                const double B0 = xadd.get();

                xadd.free();
                for(const anode *node=working.head;node;node=node->next)
                {
                    const species &s = **node;
                    const size_t   j = *s;
                    const int      d = lam[j];
                    const double   c = Ctry[j];
                    if(d&&c<0) xadd << -c;
                }
                const double B1 = xadd.get();
                std::cerr << "\tB0   = " << B0   << std::endl;
                std::cerr << "\tCbal = " << Cbal << std::endl;
                std::cerr << "\tCtry = " << Ctry << std::endl;
                std::cerr << "\tB1   = " << B1   << std::endl;
                const double res = B0-B1;
                if(res>0)
                {
                    cf =  a.f;
                    sz = &a.s;
                }
                return res;
            }

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

            vector<int> xd(L,0);
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
                                beta[j] = 0;
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

            // computing xd = NuL * beta
            {
                //NuA.assign(Nu);
                int xmax = 0;
                for(const enode *node=lattice.head();node;node=node->next)
                {
                    const equilibrium &  eq = ***node;
                    const size_t         ei = *eq;
                    const readable<int> &nu = NuL[ei];
                    int                 &xx = xd[ei];
                    xx   = iota::dot<int>::of(nu,beta);
                    xmax = max_of(xmax,absolute(xx));

                    if(verbose) lattice.pad(std::cerr << "| <" << eq.name << ">",eq) << " = " << std::setw(4) << xx << std::endl;


                }
                YACK_XMLOG(xml,"-- |xmax| = " << xmax);
            }

            // computing Lambda matrix = NuL' * diagm(xd)
            std::cerr << "xd =" << xd  << std::endl;

            imatrix               Lambda(L,M);
            vector<equilibrium *> alive(L,as_capacity);

            for(const enode *node=lattice.head();node;node=node->next)
            {
                const equilibrium   &eq    = ***node;
                const size_t         ei    = *eq;
                const int            d = xd[ei];
                writable<int>       &l = Lambda[ei];
                const readable<int> &n = NuL[ei];
                int lmax = 0;
                for(size_t j=M;j>0;--j)
                {
                    lmax = max_of(lmax, absolute( l[j] = n[j] * d ) );
                }
                if(lmax) alive << &coerce(eq);
            }


            std::cerr << "Lambda = " << Lambda << std::endl;
            std::cerr << "C      = " << Cbal   << std::endl;
            std::cerr << "@alive = " << alive.size() << "/" << L << std::endl;

            const size_t na = alive.size();



            for(size_t ia=1;ia<=na;++ia)
            {
                const equilibrium   &eq    = *alive[ia];
                const size_t         ei    = *eq;
                const readable<int> &lam   = Lambda[ei];
                if(verbose) lattice.pad(std::cerr << '<' << eq.name <<'>',eq) << " : " << lam << std::endl;

                const species *s = NULL;
                double         f = 0;
                const double   g = gain(lam,f,s);
                std::cerr << "\t\tgain=" << g << std::endl;

            }
            




            exit(0);
            return false;
        }




    }

}


