
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

    static inline void create(writable<int> &nu, randomized::bits &ran)
    {
        for(size_t i=nu.size();i>0;--i)
        {
            nu[i] = static_cast<int>( ran.in(-3,3) );
        }
    }
    
    static inline void create(matrix<int> &nu, randomized::bits &ran)
    {
        for(size_t i=1;i<=nu.rows;++i)
        {
            create(nu[i],ran);
        }
    }
    


}

YACK_UTEST(apex_north_space)
{
    randomized::rand_ ran;

    const size_t n = 4;
    
    matrix<int>    nu(n,n+4);
    vector<int>    cf(nu.cols);
    do
    {
        create(nu,ran);
    } while( apk::rank_of(nu) < n);

    std::cerr << "using " << nu << std::endl;

    for(size_t k=2;k<n;++k)
    {
        // create starting family
        combination    comb(n,k);
        vector<size_t> indx(k);
        vector<size_t> left(n-k);
        
        do
        {
            // choose another ordering
            do {
                for(size_t i=k;i>0;--i)
                    indx[i] = comb[i];
                randomized::shuffle::tableau(indx,ran);
            } while( indx == comb );
            
            for(size_t i=1,j=1;i<=n;++i)
            {
                if( comb.look_up(i) ) continue;;
                left[j++] = i;
            }
            
            
            
            std::cerr << "using " << comb << "/" << indx << " | left=" << left << std::endl;

            north::qmatrix U(nu.cols), V(nu.cols);
            for(size_t i=1;i<=k;++i)
            {
                YACK_ASSERT(U.grow(nu[comb[i]]));
                YACK_ASSERT(V.grow(nu[indx[i]]));
            }
            std::cerr << "U=" << U << std::endl;
            std::cerr << "V=" << V << std::endl;
            std::cerr << "[";
            for(size_t iter=10;iter>0;--iter)
            {
                north::qmatrix UU(U);
                north::qmatrix VV(V);
                do
                {
                    create(cf,ran);
                }
                while( !UU.grow(cf) );
                YACK_ASSERT(VV.grow(cf));
                std::cerr << ".";
                YACK_ASSERT( UU.last() == VV.last() );
                //std::cerr << UU.last() << " / " << VV.last() << std::endl;
            }
            std::cerr << "]" << std::endl;
            
            

        } while(comb.next());

    }







    
}
YACK_UDONE()
