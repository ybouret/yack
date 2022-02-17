#include "yack/chem/equilibrium.hpp"
#include "yack/type/utils.hpp"
#include <cmath>

namespace yack
{
    namespace chemical
    {

        
        void equilibrium::  solve(const double      K0,
                                  writable<double> &C,
                                  writable<double> &Ctry) const
        {
            switch(kind)
            {
                case family_0_1: (**prod.head)(C) = K0;       return;
                case family_1_0: (**reac.head)(C) = 1.0/K0;   return;
                case family_0_2: (**prod.head)(C) = sqrt(K0); return;
                case family_1_1: {
                    double &A = (**reac.head)(C);
                    double &B = (**prod.head)(C);
                    A         = (A+B)/(1.0+K0);
                    B         = K0*A;
                } return;

                case family_0_11: {
                    double &A = (**prod.head)(C);
                    double &B = (**prod.tail)(C);
                    const double AmB = A-B;
                    const double sqd = sqrt(4.0 * K0 + squared(AmB));
                    A = max_of(0.5*(sqd+AmB),0.0);
                    B = max_of(0.5*(sqd-AmB),0.0);
                } return;

                case family_any:
                    break;
            }

            const species *vanishing = NULL;
            const double   xi        = extent(K0,C,Ctry,vanishing);
            move(C,C,xi);
            if(vanishing) (*vanishing)(C) = 0;
        }


    }
}
