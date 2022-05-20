#include "yack/apex/kernel.hpp"

namespace yack
{

    apn apk:: lcm(const matrix<apq> &M)
    {
        if(M.rows)
        {
            return lcm<matrix<apq>,size_t>(M,1,M.cols,1,M.rows);
        }
        else
        {
            return 1;
        }
    }

}
