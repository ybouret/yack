
//! \file

#ifndef YACK_MATH_SVD_INCLUDED
#define YACK_MATH_SVD_INCLUDED 1

#include "yack/sequence/arrays.hpp"
#include "yack/container/matrix.hpp"

namespace yack
{
    namespace math
    {

        //______________________________________________________________________
        //
        //
        //! Singular Values Decomposition
        //
        //______________________________________________________________________
        template <typename T>
        class svd : public arrays_of<T>
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef arrays_of<T>                  tableaux;   //!< alias
            typedef typename tableaux::array_type array_type; //!< alias
            static const size_t MAX_ITS = 1024; //!< maximum iterations

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit svd(const size_t dims=0); //!< setup
            virtual ~svd() throw();            //!< cleanup

            //__________________________________________________________________
            //
            // method
            //__________________________________________________________________

            //! singular values decomposition
            /**
             - Given a matrix a[1..m][1..n], this routine computes its singular value
             decomposition, a = u.w.v'.
             - The matrix u replaces a on output.
             - The diagonal matrix of singular values w is output as a vector w[1..n].
             - The matrix v (not the transpose v') is output as v[1..n][1..n].
             */
            bool build(matrix<T> &a, writable<T> &w, matrix<T> &v);

            //! singular value solve u.w.v' x = b
            void solve(const matrix<T> &u, const readable<T> &w, const matrix<T> &v, writable<T> &x, const readable<T> &b);

#if 0
            //! return nullity of original matrix  w.r.t ftol
            static size_t nullity(writable<T> &w, const T ftol) throw();

            //! return automatic nullity: ftol = w.size()*wmax*numeric<T>::epsilon
            static size_t nullity(writable<T> &w) throw();
#endif
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(svd);
            array_type &rv1; //!< a.cols=n
        };

    }
}

#endif
