//! \file

#ifndef YACK_CROUT__INCLUDED
#define YACK_CROUT__INCLUDED 1

#include "yack/container/dynamic.hpp"
#include "yack/large-object.hpp"

namespace yack
{
    namespace math
    {
        //______________________________________________________________________
        //
        //
        //! data for Crout's algorithm
        //
        //______________________________________________________________________
        class crout_ : public large_object, public dynamic
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
           
            //! setup raw memory
            explicit crout_(const size_t dimension,
                            const size_t type_size,
                            const size_t scal_size);
            
            //! cleanup
            virtual ~crout_() throw();
            
            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            virtual size_t granted() const throw();
            
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const   size_t nmax;  //!< maximum dimension
            const   bool   dneg;  //!< marker for negative det

        protected:
            size_t        *indx_; //!< for thin_array<size_t>
            void          *scal_; //!< for scalars
            void          *xtra_; //!< for extra row/col

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(crout_);
            void   *wksp;
            size_t  wlen;
        };
    }
}

#endif

