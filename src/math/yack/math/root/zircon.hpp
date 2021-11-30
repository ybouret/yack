
//! \file

#ifndef YACK_ROOT_ZIRCON_INCLUDED
#define YACK_ROOT_ZIRCON_INCLUDED 1

#include "yack/container/matrix.hpp"
#include "yack/math/fcn/jacobian.hpp"

namespace yack
{

    namespace math
    {
        //______________________________________________________________________
        //
        //
        //! zircon
        //
        //______________________________________________________________________
        template <typename T>
        class zircon : public arrays_of<T>
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef arrays_of<T>                  tableaux;   //!< alias
            typedef typename tableaux::array_type array_type; //!< alias

            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            virtual ~zircon() throw(); //!< cleanup
            explicit zircon(const size_t dims=0); //!< setup

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            array_type &X;  //!< current position
            array_type &F;  //!< current values
            array_type &G;  //!< current control gradient
            array_type &V;  //!< temporary vector
            T           f0; //!< current control value F^2/2

            matrix<T>   J;     //!< jacobian
            matrix<T>   Jt;    //!< current jacobian
            matrix<T>   iJ;    //!< decomposed jacobian
            jacobian<T> fdjac; //!< finite difference jacobian
            
            //! X is set
            /**
             * - userF(F,X)
             * - userJ(J,X)
             */
            template <typename FUNCTION,
            typename JACOBIAN>
            void load(FUNCTION &userF, JACOBIAN &userJ)
            {
                userF(F,X);
                userJ(J,X);
            }




        private:
            YACK_DISABLE_COPY_AND_ASSIGN(zircon);
            void setup(size_t dims);
            void analyze();
            T    objective(const array_type &FF) throw();
            

        };

    }

}

#endif
