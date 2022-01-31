#include "yack/chem/equilibrium.hpp"
#include "yack/type/utils.hpp"
#include "yack/exception.hpp"

#include "yack/math/root/zrid.hpp"

namespace yack
{
    namespace chemical
    {
        using namespace math;

        namespace {

            // wrapper to build a changed concentration
            struct eqzcall
            {
                const equilibrium       &eq;
                const double             K0;
                const readable<double>  &C;
                writable<double>        &Ctry;

                double operator()(const double xi)
                {
                    for(const cnode *node=eq.head();node;node=node->next)
                    {
                        const component &cm = ***node;
                        const species   &sp = cm.sp;
                        const size_t     j  = sp.indx; assert(j>=1); assert(j<=C.size()); assert(j<=Ctry.size());
                        const unit_t     nu = cm.nu;
                        const double     Cj = C[j];    assert(C[j]>=0);
                        Ctry[j] = max_of<double>(Cj + nu * xi,0);
                    }
                    return eq.mass_action(K0,Ctry);
                }
            };

        }

        void equilibrium::  solve(const double      K0,
                                  writable<double> &C,
                                  writable<double> &Ctry)
        {

            const limits      &lm  = find_limits(C);
            std::cerr << lm << std::endl;
            triplet<double>    x  = { 0, 0, 0 };            // extents
            triplet<double>    f  = { 0, 0, 0 };            // mass action
            eqzcall            F  = { *this, K0, C, Ctry }; // callable

            switch(lm.type)
            {
                case limited_by_both:
                    f.a = F(x.a = -lm.prod->xi);
                    f.c = F(x.c = lm.reac->xi);
                    break;
                case limited_by_reac:
                    f.c = F(x.c = lm.reac->xi);
                    f.a = F(x.a = -1);
                    break;

                case limited_by_prod:
                    f.a = F(x.a = -lm.prod->xi);
                    f.c = F(x.c = 1);
                    break;

                case limited_by_none:
                    f.a = F(x.a=-1);
                    f.c = F(x.c=1);
                    break;
            }

            while(f.a*f.c>0)
            {
                x.a -= 1; x.a *= 2;
                x.c += 1; x.c *= 2;
                f.a  = F(x.a);
                f.c  = F(x.c);
            }


            zrid<double>  Z;
            if(!Z(F,x,f)) throw exception("can't solve corrupted <%s>", name() );

            for(const cnode *node=head();node;node=node->next)
            {
                const component &cm = ***node;
                const size_t     j  = cm.sp.indx;
                C[j] = Ctry[j];
            }
            assert( std::abs(mass_action(K0,C)-f.b) <= 0 );

        }
    }
}
