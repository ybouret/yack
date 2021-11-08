
//! \file

#ifndef YACK_MATH_LU__INCLUDED
#define YACK_MATH_LU__INCLUDED 1

#include "yack/type/scalar.hpp"
#include "yack/container/matrix.hpp"
#include "yack/sequence/thin-array.hpp"
#include "yack/math/api.hpp"

namespace yack
{
    namespace math
    {
        //______________________________________________________________________
        //
        //
        //! memory for LU decomposition
        //
        //______________________________________________________________________
        class lu_ : public writable<size_t>, public dynamic
        {
        public:
            //__________________________________________________________________
            //
            // definitions
            //__________________________________________________________________
            typedef void (*proc)(void*); //!< create/delete one type

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            virtual               ~lu_()                          throw(); //!< cleanup
            virtual size_t         size()                   const throw(); //!< dims
            virtual size_t &       operator[](const size_t)       throw(); //!< in [1..dims]
            virtual const size_t & operator[](const size_t) const throw(); //!< in [1..dims]
            virtual size_t         granted()                const throw(); //!< allocated bytes

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const size_t dims; //!< maximal dimensions solver

        protected:
            bool         dneg; //!< signature for sign change of determinant

            //! allocate memory and create dims object
            explicit lu_(const size_t nmax,
                         const size_t itsz,
                         proc         make,
                         proc         done);

            //! address for scal
            template <typename T> T *scal_() throw()
            {
                return coerce_cast<T>(data);
            }


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(lu_);
            size_t      *upos; //!< [1..size]
            uint8_t     *data; //!< first byte of data
            const size_t step; //!< sizeof(type)
            proc         kill; //!< ~type()
            void        *wksp; //!< memory
            size_t       wlen; //!< granted
            void         clear(size_t) throw();
        };
        
    }
}


#endif

