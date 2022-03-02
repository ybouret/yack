
#include "yack/chem/components.hpp"
#include "yack/math/triplet.hpp"

namespace yack
{
    namespace chemical
    {



        double components:: extent(const double            K,
                                   const readable<double> &C) const
        {

            const limits &lim = private_limits(C);
            std::cerr << lim << std::endl;
            return 0;

        }

    }

}
