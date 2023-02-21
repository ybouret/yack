//! \file

#ifndef YACK_MATH_JACOBI_INCLUDED
#define YACK_MATH_JACOBI_INCLUDED 1

#include "yack/sequence/arrays.hpp"
#include "yack/container/matrix.hpp"

namespace yack
{
    namespace math
    {
        //! post diagonalisation sorting kind
        enum sort_eigv_by
        {
            sort_eigv_none,       //!< no sorting
            sort_eigv_by_value,   //!< sort by decreasing value
            sort_eigv_by_module   //!< sort by decreasing module
        };

        //______________________________________________________________________
        //
        //
        //! jacobi transform of a symmetric matrix
        //
        //______________________________________________________________________
        template <typename T>
        class jacobi : public arrays_of<T>
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef arrays_of<T>                  tableaux;   //!< alias
            typedef typename tableaux::array_type array_type; //!< alias
            static const size_t min_iter = 4;                 //!< for tolerance scaling
            static const size_t max_iter = 64;                //!< algorithm should converge before

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit jacobi(const size_t n=0); //!< setup
            virtual ~jacobi() noexcept;         //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            //! Jacobi reduction
            /**
             \param a    is a symetric matrix, REGENERATED at the end...
             \param d    are the eigenvalues
             \param v    columns are the eigenvectors
             \param kind how to sort vectors/values afterwards
             \return a =  v * d * v'
             */
            bool  operator()( matrix<T> &a, writable<T> &d, matrix<T> &v, sort_eigv_by kind = sort_eigv_none );


            //! sort by decreasing value
            static void  eigsrt(writable<T> &d,  matrix<T> &v) noexcept;

            //! sort by decreasing module
            static void  eigsrtA(writable<T> &d, matrix<T> &v) noexcept;


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(jacobi);
            array_type &b;
            array_type &z;
        };


    }

}

#endif
