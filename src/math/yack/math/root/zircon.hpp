
//! \file

#ifndef YACK_ROOT_ZIRCON_INCLUDED
#define YACK_ROOT_ZIRCON_INCLUDED 1

#include "yack/container/matrix.hpp"
#include "yack/math/fcn/jacobian.hpp"
#include "yack/math/algebra/svd.hpp"

namespace yack
{

    namespace math
    {

        namespace core
        {
            class zircon
            {
            public:

                enum topology
                {
                    regular,
                    degenerate,
                    singular
                };

                virtual ~zircon() throw();


            protected:
                explicit zircon() throw();

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(zircon);
            };
        }

        //______________________________________________________________________
        //
        //
        //! zircon
        //
        //______________________________________________________________________
        template <typename T>
        class zircon : public core::zircon, public arrays_of<T>
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
            array_type &X;     //!< current position
            array_type &F;     //!< current values
            array_type &G;     //!< current control gradient
            array_type &W;     //!< singular values
            array_type &S;     //!< computed step
            array_type &XX;    //!< temporary vector
            array_type &FF;    //!< temporary vector
            array_type &VV;    //!< temporary vector
            T           f0;    //!< current control value F^2/2
            matrix<T>   J;     //!< jacobian
            matrix<T>   Jt;    //!< current jacobian
            matrix<T>   U;     //!< for svd
            matrix<T>   V;     //!< for svd
            svd<T>      study; //!< study definition by svd
            jacobian<T> fdjac; //!< finite difference jacobian, with inital scaling

            //! prepare memory
            void setup(size_t dims);


            //! X is set
            /**
             * - userF(F,X)
             * - userJ(J,X)
             * - study J
             * - if not singular, the corresponding full step=S
             *   is computed along with the estimated position XX=X+S
             */
            template <typename FUNCTION, typename JACOBIAN>
            topology load(FUNCTION &userF, JACOBIAN &userJ)
            {
                userF(F,X);
                userJ(J,X);
                return initialize();
            }

            //! X is set
            /**
             load values with internal jacobian computation
             */
            template <typename FUNCTION>
            topology load(FUNCTION &userF)
            {
                jwrap<FUNCTION> userJ = { fdjac, userF };
                return load(userF,userJ);
            }

            template <typename FUNCTION>
            void forward(FUNCTION &userF)
            {
                std::cerr << "<forward>" << std::endl;
                userF(FF,XX);
                T f1 = objective(FF);
                std::cerr << "XX=" << XX << std::endl;
                std::cerr << "FF=" << FF << std::endl;
                std::cerr << "f1=" << f1 << ", df=" << f0-f1 << std::endl;
                
            }


            

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(zircon);


            topology initialize(); //!< compute values from F and J
            T        objective(const array_type &Ftmp) throw(); //!< |Ftmp^2|/2


            template <typename FUNCTION>
            struct jwrap
            {
                jacobian<T> &jac;
                FUNCTION    &fcn;
                inline void operator()(matrix<T> &JJ, const readable<T> &XX) { jac(JJ,fcn,XX); }
            };


        };

    }

}

#endif
