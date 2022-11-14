#include "yack/chem/reactor.hpp"
#include "yack/ios/xmlog.hpp"
#include "yack/type/utils.hpp"
#include "yack/math/opt/optimize.hpp"
#include "yack/math/iota.hpp"
#include <iomanip>
#include <cfloat>
#include "yack/type/boolean.h"
#include "yack/exception.hpp"

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
                if(c<0) xadd <<  (-c);
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
                const double B0 = xadd.get();
                std::cerr << singles << std::endl;
                std::cerr << "B0="   << B0   << std::endl;
                std::cerr << "beta=" << beta << std::endl;

                // computing xi direction
                NuA.assign(Nu);
                iota::mul(d_xi,Nu,beta);
                
                // cutting
                for(const enode *node=singles.head();node;node=node->next)
                {
                    const equilibrium &eq = ***node;
                    const size_t       ei = *eq;
                    int               &xx = d_xi[ei];
                    if(verbose)  singles.pad(std::cerr << eq.name,eq) << " @" << std::setw(3) << xx << " : ";


                    if(xx>0)
                    {
                        const xlimits     &lm = eq.primary_limits(Cbal,corelib.maxlen);
                        if(verbose) std::cerr << lm;
                    }
                    else
                    {
                        if(xx<0)
                        {
                            const xlimits     &lm = eq.primary_limits(Cbal,corelib.maxlen);
                            if(verbose) std::cerr << lm;
                        }
                        else
                        {
                            // not used...
                            NuA[ei].ld(0);
                            std::cerr << "<unused>";
                        }
                    }

                    if(verbose) std::cerr << std::endl;


                }

                exit(0);

                return false;
            }

        }


#if 0
        template <typename T>
        static inline
        std::ostream & show_bal( std::ostream &os, const readable<T> &bal)
        {
            static const unsigned w = 3;
            os<< '[' << std::setw(w) << bal[1];
            for(size_t i=2;i<=bal.size();++i)
            {
                os << ';' << std::setw(w) << bal[i];
            }
            os << ']';
            return os;
        }



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
            
            
            const bool well = isWellBalanced();

            if(well)
            {
                YACK_XMLOG(xml,"-- well balanced");
                working.transfer(C0,Cbal);
                return true;
            }
            else
            {
                const double B0 = xadd.get();
                std::cerr << singles << std::endl;
                std::cerr << "B0="   << B0   << std::endl;
                std::cerr << "beta=" << beta << std::endl;

                int     betaMax = 0;
                for(size_t i=N;i>0;--i)
                {
                    const int NuBeta = xadd.dot(Nu[i],beta);
                    betaMax = max_of(betaMax,absolute(NuBeta));
                    std::cerr << "NuBeta[" << i << "]=" << NuBeta << std::endl;
                    for(size_t j=M;j>0;--j)
                    {
                        Bal[j][i] = Nu[i][j] * NuBeta;
                    }
                }
                //std::cerr << "betaMax=" << betaMax << std::endl;
                //std::cerr << "A=" << A << std::endl;
                callB B = { *this };
                assert( fabs(B0 - B(0)) <= 0);


                blocked.ld(false); // alpha
                vector<bool>   used(M,true);
                vector<size_t> cond(M,as_capacity);

                goto COMPUTE_FIRST;

            COMPUTE_AGAIN:
                std::cerr << "-------- recomputing --------" << std::endl;
            COMPUTE_FIRST:
                cond.free();
                bool recompute = false;
                for(const anode *node=working.head;node;node=node->next)
                {
                    const species       &s   = **node;
                    const size_t         j   = *s;      if(!used[j]) continue;
                    const double         c   = Cbal[j];
                    const double         rhs = fabs(c) > 0 ? -c : 0;
                    const readable<int> &bal = Bal[j];


                    size_t ilast = 0;
                    int    clast = 0;
                    size_t ncoef = 0;
                    for(size_t i=N;i>0;--i)
                    {
                        const int coef = bal[i];
                        if(coef)
                        {
                            clast = coef;
                            ilast = i;
                            ++ncoef;
                        }
                    }

                    if(verbose)
                    {
                        corelib.pad(std::cerr << '[' << s.name << ']',s) << " : ";
                        show_bal(std::cerr,bal);
                        std::cerr << " >= " << std::setw(15) << rhs;
                        std::cerr << " | #=" << ncoef;
                    }

                    switch(ncoef)
                    {
                            //--------------------------------------------------
                        case 0: // detecting an unsused species
                            //--------------------------------------------------
                            used[j]   = false;
                            if(rhs>0)
                            {
                                if(verbose) std::cerr << " =>  inconsistent [" << s.name << "] !!" << std::endl;
                                return false;
                            }
                            else
                            {
                                if(verbose)  std::cerr << " | untouched" << std::endl;
                            }
                            break;

                            //--------------------------------------------------
                        case 1: // detecting a primary alpha
                            //--------------------------------------------------
                            if(verbose) std::cerr << " | primary";
                            if(clast>0)
                            {
                                //----------------------------------------------
                                // increasing
                                //----------------------------------------------
                                if(verbose) std::cerr << "/increasing";
                                if(rhs<=0)
                                {
                                    if(verbose) std::cerr << " | true" << std::endl;
                                    used[j]  = false;
                                    recompute = true;
                                }
                                else
                                {
                                    if(verbose) std::cerr << " | keep" << std::endl;
                                    cond << j;
                                }
                            }
                            else
                            {
                                //----------------------------------------------
                                // decreasing
                                //----------------------------------------------
                                if(verbose) std::cerr << "/decreasing";
                                switch( __sign::of(rhs) )
                                {
                                    case __zero__:
                                        if(verbose) std::cerr << " => blocking" << std::endl;
                                        blocked[ilast] =  true;
                                        for(size_t j=M;j>0;--j)
                                        {
                                            Bal[j][ilast] = 0;
                                        }
                                        used[j] = false;
                                        goto COMPUTE_AGAIN;

                                    case negative:
                                        if(verbose) std::cerr <<  " => limiting" << std::endl;
                                        cond << j;
                                        break;

                                    case positive:
                                        if(verbose) std::cerr << " =>  inconsistent [" << s.name << "] !!" << std::endl;
                                        return false;
                                }

                            }
                            break;

                            //--------------------------------------------------
                        default: // generic
                            //--------------------------------------------------
                            std::cerr << " | generic replica" << std::endl;
                            cond << j;
                            break;

                    }
                }

                if(recompute) goto COMPUTE_AGAIN;


                if(verbose)
                {
                    std::cerr << "cond=" << cond << std::endl;
                }


                exit(1);





            }
            
            return false;
            
        }
#endif

    }

}

