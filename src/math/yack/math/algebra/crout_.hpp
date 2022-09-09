//! \file

#ifndef YACK_CROUT_INCLUDED
#define YACK_CROUT_INCLUDED 1

#include "yack/container/dynamic.hpp"
#include "yack/sequence/thin-array.hpp"

namespace yack
{
    namespace math
    {
        class crout_ : public dynamic
        {
        public:
            const   size_t dims;

            explicit crout_(const size_t dimension,
                            const size_t type_size,
                            const size_t scal_size);
            virtual ~crout_() throw();

            virtual size_t granted() const throw();
            
        protected:
            size_t *indx_; //!< for thin_array<size_t>
            void   *scal_; //!< for scalars
            void   *xtra_; //!< for extra row/col

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(crout_);
            void   *wksp;
            size_t  wlen;
        };
    }
}

#endif

