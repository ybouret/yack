#include "yack/chem/equilibrium.hpp"


namespace yack
{
    namespace chemical
    {

        
        void equilibrium::  solve(const double      K0,
                                  writable<double> &C,
                                  writable<double> &Ctry) const
        {
            const species *vanishing = NULL;
            const double   xi        = extent(K0,C,Ctry,vanishing);
            move(C,Ctry,xi);
            if(vanishing) (*vanishing)(C) = 0;
        }


    }
}
