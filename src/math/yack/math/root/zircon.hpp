
//! \file

#ifndef YACK_ROOT_ZIRCON_INCLUDED
#define YACK_ROOT_ZIRCON_INCLUDED 1

#include "yack/container/matrix.hpp"
#include "yack/math/fcn/jacobian.hpp"
#include "yack/math/algebra/svd.hpp"
#include "yack/ios/ocstream.hpp"
#include "yack/math/triplet.hpp"
#include <cmath>

namespace yack
{

    namespace math
    {

        namespace core
        {
            //! base class
            class zircon
            {
            public:

                //! result from analyze
                enum topology
                {
                    regular,
                    degenerate,
                    singular
                };

                virtual ~zircon() throw(); //!< cleanup

                bool verbose;              //!< verbosity
                
            protected:
                explicit zircon() throw(); //!< setup

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(zircon);
            };
        }

        //! helper to trace
#define YACK_ZIRCON_PRINTLN(MSG) do { if(verbose) { std::cerr << MSG << std::endl; } } while(false)

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
            //! 1D function interface
            //__________________________________________________________________
            class f1d {
            public:
                virtual ~f1d() throw() {}

                virtual T operator()(const T) = 0;

            protected:
                explicit f1d() throw() {}

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(f1d);
            };
            
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
             * - forward to XX
             */
            template <typename FUNCTION, typename JACOBIAN>
            topology step(FUNCTION &userF, JACOBIAN &userJ)
            {
                userF(F,X); // F=userF(X)
                userJ(J,X); // J=userJ(X);
                fwrap<FUNCTION> F1d(userF,*this);
                return lookup(F1d);
            }

            //! X is set
            /**
             load values with internal jacobian computation
             */
            template <typename FUNCTION>
            topology step(FUNCTION &userF)
            {
                jwrap<FUNCTION> userJ = { fdjac, userF };
                return step(userF,userJ);
            }

            T                 control(const array_type &Ftmp) throw(); //!< |Ftmp^2|/2
            const array_type &probe(const T u) throw();                //!< XX = X + u * S
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(zircon);

            topology lookup(f1d &f);


            template <typename FUNCTION>
            struct jwrap
            {
                jacobian<T> &jac;
                FUNCTION    &fcn;
                inline void operator()(matrix<T> &JJ, const readable<T> &XX) { jac(JJ,fcn,XX); }
            };

            template <typename FUNCTION>
            class fwrap : public f1d
            {
            public:
                inline virtual ~fwrap() throw() {}
                inline explicit fwrap(FUNCTION &user_f,
                                      zircon   &myself) throw() :
                func(user_f),
                self(myself)
                {
                }

                FUNCTION         &func;
                zircon           &self;

                inline virtual T operator()(const T u)
                {
                    func(self.FF,self.probe(u));
                    return self.control(self.FF);
                }

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(fwrap);
            };


        };

    }

}

#endif
