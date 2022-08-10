#include "yack/chem/mix/independency.hpp"
#include "yack/math/algebra/lu.hpp"

namespace yack
{
    namespace chemical
    {
        bool independency:: verify(matrix<apq> &G)
        {
            math::lu<apq> LU(G.rows);
            return LU.build(G);
        }

    }
}
