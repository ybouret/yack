//! \file

#ifndef YACK_MATH_LU_KERNEL_INCLUDED
#define YACK_MATH_LU_KERNEL_INCLUDED 1

#include "yack/large-object.hpp"
#include "yack/container/dynamic.hpp"

namespace yack
{
    namespace math
    {

        //______________________________________________________________________
        //
        //
        //! common fields for LU
        //
        //______________________________________________________________________
        class LU_Kernel : public dynamic
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~LU_Kernel() noexcept; //!< cleanup

            //! linear memory for maximum dimension and type sizes
            explicit LU_Kernel(const size_t maximum_dim,
                               const size_t scalar_size,
                               const size_t inside_size);
            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            virtual size_t granted() const noexcept; //!< linear memory

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const size_t nmax; //!< maximum size to solve
            const bool   dneg; //!< keeping track of determinant sign


        protected:
            size_t       *indx_; //!< memory for indx    [nmax]
            void         *scal_; //!< memory for scalars [nmax * scal_size]
            void         *xtra_; //!< memory for extra row/col [nmax * type_size*2]

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(LU_Kernel);
            void  *wksp;
            size_t wlen;
        };

    }
}

#endif

