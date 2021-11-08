
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
        //! linear memory for LU decomposition
        //
        //______________________________________________________________________
        class lu_ :  public dynamic
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
                         const size_t s_sz,
                         const size_t t_sz);

            //! address of indices
            size_t *indx_() throw();

            //! address for scal
            template <typename T> inline T *scal_() throw()
            {
                return coerce_cast<T>(sdat);
            }

            //! address for extra row
            template <typename T> inline T *xrow_() throw()
            {
                return coerce_cast<T>(tdat);
            }


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(lu_);
            size_t      *upos; //!< [1..size]
            uint8_t     *sdat; //!< first byte for scalar data
            uint8_t     *tdat; //!< first byte for object data
            void        *wksp; //!< memory
            size_t       wlen; //!< granted
        };
        
    }
}


#endif

