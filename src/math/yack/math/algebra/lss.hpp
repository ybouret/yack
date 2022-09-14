
//! \file

#ifndef YACK_MATH_LSS_INCLUDED
#define YACK_MATH_LSS_INCLUDED 1

#include "yack/math/algebra/crout.hpp"
#include "yack/ptr/auto.hpp"

namespace yack
{
    namespace math
    {

        //______________________________________________________________________
        //
        //
        //! dynamic linear system solver
        //
        //______________________________________________________________________
        template <typename T>
        class lss : public auto_ptr< crout<T> >
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef crout<T>            algo_type;
            typedef auto_ptr<algo_type> self_type;
            typedef adder<T>            xadd_type;

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~lss() throw()                   {} //!< cleanup>
            explicit lss() throw() : self_type(NULL) {} //!< create empty
            explicit lss(const size_t nmax) : self_type( new algo_type(nmax) ) {} //!< create with capacity

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! ensure minimal dimensions
            void ensure(const size_t nmin)
            {
                self_type &self = *this;
                if(self->nmax < nmin)
                {
                    self = new algo_type(nmin);
                }
            }

            //! decompose a matrix with proper size
            bool build(matrix<T> &a)
            {
                assert(a.is_square());
                assert(a.rows>0);
                ensure(a.rows);
                return (**this).build(a);
            }

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(lss);
        };

    }

}

#endif
