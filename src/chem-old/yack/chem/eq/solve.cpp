#include "yack/chem/equilibrium.hpp"
#include "yack/type/utils.hpp"
#include "yack/sort/sum.hpp"
#include <cmath>

namespace yack
{
    namespace chemical
    {

        
        void equilibrium::  solve(const double      K0,
                                  writable<double> &C0,
                                  writable<double> &Ctry) const
        {
#if 0
            switch(topo)
            {
                case topology_0_1: (**prod.head)(C0) = K0;       return;
                case topology_1_0: (**reac.head)(C0) = 1.0/K0;   return;
                case topology_0_2: (**prod.head)(C0) = sqrt(K0); return;
                case topology_1_1: {
                    double &A = (**reac.head)(C0);
                    double &B = (**prod.head)(C0);
                    A         = (A+B)/(1.0+K0);
                    B         = K0*A;
                } return;

                case topology_0_11: {
                    double &A = (**prod.head)(C0);
                    double &B = (**prod.tail)(C0);
                    const double AmB = A-B;
                    const double sqd = sqrt(4.0 * K0 + squared(AmB));
                    A = max_of(0.5*(sqd+AmB),0.0);
                    B = max_of(0.5*(sqd-AmB),0.0);
                } return;

                case topology_1_11: {
                    double &A = (**reac.head)(C0);
                    double &B = (**prod.head)(C0);
                    double &C = (**prod.tail)(C0);
                    const double Delta = squared(K0) + squared(B-C) + twice(K0*(sorted::sum(twice(A),B,C)));
                    const double xi    = 0.5*(sqrt(Delta)-sorted::sum(K0,B,C));
                    A = max_of(A-xi,0.0);
                    B = max_of(B+xi,0.0);
                    C = max_of(C+xi,0.0);
                } return;

                case topology_11_11: {
                    double       &A = (**reac.head)(C0);
                    double       &B = (**reac.tail)(C0);
                    double       &C = (**prod.head)(C0);
                    double       &D = (**prod.tail)(C0);
                    const double AB = A*B;
                    const double CD = C*D;
                    if(fabs(AB)>0||fabs(CD)>0)
                    {
                        const double mb    = K0*(A+B)+(C+D);
                        const double gam   = K0*AB - CD;
                        const double Delta = max_of(mb*mb - 4.0 * (K0-1.0) * gam,0.0);
                        const double xi    =  twice( gam / (mb+sqrt(Delta)) );
                        A = max_of(A-xi,0.0);
                        B = max_of(B-xi,0.0);
                        C = max_of(C+xi,0.0);
                        D = max_of(D+xi,0.0);
                        return;
                    }
                    else
                    {
                        // nothing
                        return;
                    }
                }

                case topology_any:
                    break;
            }
#endif
            
            const double   xi_ = extent_(K0,C0,Ctry);
            move(C0,C0,xi_);
        }


    }
}
