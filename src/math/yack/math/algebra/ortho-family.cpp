
#include "yack/math/algebra/ortho-family.hpp"
#include "yack/apex/kernel.hpp"
#include "yack/math/iota.hpp"
#include "yack/math/algebra/crout.hpp"

namespace yack
{
    namespace math
    {
        
        bool ortho_family:: make(matrix<apq>       &Q,
                                 const matrix<apq> &P)
        {
            const size_t n = P.rows;
            const size_t m = P.cols;
            assert(Q.rows==m);
            assert(Q.cols==m);
            if(n<=0)
            {
                assert(0==m);
                return true;
            }
            
            matrix<apq> iP(P);
            matrix<apq> tP(P,transposed);
            {
                matrix<apq> P2(n,n);
                iota::gram(P2,P);
                crout<apq> lu(n);
                if(!lu.build(P2)) {
                    return false;
                }
                lu.solve(P2,iP);
            }
            iota::mmul(Q,tP,iP);
            const apq _1 = 1;
            for(size_t i=m;i>0;--i)
            {
                for(size_t j=m;j>i;--j)
                {
                    Q[i][j] = -Q[i][j];
                }
                Q[i][i] = _1 - Q[i][i];
                for(size_t j=i-1;j>0;--j)
                {
                    Q[i][j] = -Q[i][j];
                }
            }
            
            apk::simplify_rows(Q);

            return true;
                
        }
    }
    
}
