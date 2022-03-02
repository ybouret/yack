
#include "yack/chem/components.hpp"
#include "yack/math/triplet.hpp"

namespace yack
{
    using namespace math;

    namespace chemical
    {


        double components:: extent(const double            K,
                                   const readable<double> &C) const
        {

            const limits &lim = private_limits(C);
            std::cerr << lim << std::endl;
            triplet<double> x = { 0, 0, 0 };

            switch(lim.type)
            {
                case limited_by_both:
                    x.a = -lim.prod->xi; 
                    x.b =  lim.reac->xi;
                    break;
            }


            return 0;

        }

    }

}
