//! \file

#ifndef YACK_MATH_CHOLESKY_INCLUDED
#define YACK_MATH_CHOLESKY_INCLUDED 1

#include "yack/container/matrix.hpp"
#include "yack/sequence/vector.hpp"

namespace yack
{
    namespace math
    {
        //______________________________________________________________________
        //
        //
        //! Cholesky decomposition
        //
        //______________________________________________________________________
        template <typename T>
        class cholesky : public vector<T>
        {
        public:
            //__________________________________________________________________
            //
            // types
            //__________________________________________________________________
            typedef vector<T>       base_type; //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit cholesky()       throw(); //!< default setup
            explicit cholesky(const size_t n); //!< setup with data
            virtual ~cholesky()       throw(); //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! build the decomposition of a symmetric positive matrix
            /**
             Only the upper triangle of a is used and not modified.
             The decomposition is stored in the lower triangle,
             and 'this' holds the diagonal terms
             */
            bool build(matrix<T> &a);

            //! solve after decomposition of a
            void solve(writable<T> &x, const matrix<T> &a, const readable<T> &b) const throw();


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(cholesky);
        };

    }

}

#endif

