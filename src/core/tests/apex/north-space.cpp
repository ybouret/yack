
#include "yack/apex/north/qmatrix.hpp"
#include "yack/apex/north/qselect.hpp"
#include "yack/utest/run.hpp"

#include "yack/container/matrix.hpp"
#include "yack/sequence/vector.hpp"
#include "yack/ios/ascii/convert.hpp"
#include "yack/ios/ocstream.hpp"
#include "yack/apex/kernel.hpp"
#include "yack/counting/comb.hpp"
#include "yack/randomized/shuffle.hpp"

using namespace yack;

namespace
{

    static inline void create_topo(matrix<int> &nu, randomized::bits &ran)
    {
        for(size_t i=1;i<=nu.rows;++i)
        {
            for(size_t j=1;j<=nu.cols;++j)
            {
                const double p = ran.to<double>();
                if(p<0.0) {
                    nu[i][j] = 0;
                }
                else
                {
                    nu[i][j] = static_cast<int>( ran.in(-3,3) );
                }
            }
        }
    }
    


}

YACK_UTEST(apex_north_space)
{
    randomized::rand_ ran;

    const size_t n = 5;

    matrix<int>    nu(n,n+4);
    do
    {
        create_topo(nu,ran);
    } while( apk::rank_of(nu) < n);

    std::cerr << "using " << nu << std::endl;

    for(size_t k=2;k<n;++k)
    {
        combination    comb(n,k);
        vector<size_t> indx(k);
        do
        {
            do {
                for(size_t i=k;i>0;--i)
                    indx[i] = comb[i];
                randomized::shuffle::tableau(indx,ran);
            } while( indx == comb );

            std::cerr << "using " << comb << "/" << indx << std::endl;

            north::qmatrix U(n), V(n);


        } while(comb.next());

    }







    
}
YACK_UDONE()
