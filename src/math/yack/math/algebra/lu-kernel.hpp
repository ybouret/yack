//! \file

#ifndef YACK_MATH_LU_KERNEL_INCLUDED
#define YACK_MATH_LU_KERNEL_INCLUDED 1

#include "yack/large-object.hpp"
#include "yack/container/dynamic.hpp"

namespace yack
{
    namespace math
    {

        class LU_Kernel : public large_object, public dynamic
        {
        public:
            virtual ~LU_Kernel() noexcept;
            explicit LU_Kernel(const size_t maxi_dims,
                               const size_t scal_size,
                               const size_t type_size);

            virtual size_t granted() const noexcept;


            const size_t nmax;
            const bool   dneg;


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
