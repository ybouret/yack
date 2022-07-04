#ifndef YACK_INDEPENDENCY_INCLUDED
#define YACK_INDEPENDENCY_INCLUDED 1

#include "yack/apex/rational.hpp"
#include "yack/math/iota.hpp"

namespace yack
{
    namespace chemical
    {
        //______________________________________________________________________
        //
        //! test independency of topology matrices
        //
        //______________________________________________________________________
        struct independency
        {

            //__________________________________________________________________
            //
            //! return true is rank(Nu) = Nu.rows
            //__________________________________________________________________
            template <typename T> static inline
            bool of(const matrix<T> &Nu)
            {
                const size_t n = Nu.rows;
                matrix<apq>  G(n,n);
                math::iota::gram(G,Nu);
                return verify(G);
            }
            
        private:
            static bool verify(matrix<apq> &G);
        };
    }
}

#endif

