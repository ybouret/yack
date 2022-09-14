
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

            //! create empty
            explicit lss(const bool flag) throw() :
            self_type(NULL),
            xadd(),
            fine(flag)
            {
            }


            //! create with dimension
            explicit lss(const bool flag, const size_t nmax) :
            self_type( new algo_type(nmax) ),
            xadd(),
            fine(flag)
            {}

            //! cleanup
            virtual ~lss() throw()
            {}

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! ensure minimal dimensions
            inline void ensure(const size_t nmin)
            {
                self_type &self = *this;
                if( self.is_empty() || self->nmax < nmin)
                {
                    self = new algo_type(nmin);
                }
            }

            //! decompose a matrix with proper size
            inline bool build(matrix<T> &a)
            {
                assert(a.is_square());
                assert(a.rows>0);
                ensure(a.rows);
                return fine ? (**this).build(a,xadd) : (**this).build(a);
            }


            //! solve with a decomposed matrix
            void solve(const matrix<T> &a, writable<T> &b)
            {
                if(fine) {
                    (**this).solve(a,b,xadd);
                } else   {
                    (**this).solve(a,b);
                }
            }

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            xadd_type xadd; //!< adder
            bool      fine; //!< choice of algorithm

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(lss);
        };

    }

}

#endif
