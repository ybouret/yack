
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


            protected:
                explicit zircon() throw(); //!< setup

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
            T           sigma; //!< initial negative slope
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

                const size_t n = X.size();
                // XX=X+S is computed: evaluate FF
                userF(FF,XX);

                // prepare function
                fwrap<FUNCTION> ff    = { userF, *this };
                const T         f1    = objective(FF);
                const T         slope = 0.1 * sigma;
                if(f1<=f0-slope)
                {
                    // take full step
                    for(size_t i=n;i>0;--i)
                    {
                        VV[i] = fabs(X[i]-XX[i]);
                        X[i] = XX[i];
                        F[i] = FF[i];
                    }
                }
                else
                {
                    // initialize triplet
                    triplet<T> u = { 0, T(0.5),     1  };
                    triplet<T> f = { f0, ff(u.b)  , f1 };

                    std::cerr << "u=" << u << ", f=" << f << std::endl;

                    // shrink
                    while(f.b>f.a-slope*u.b)
                    {
                        f.c = f.b;
                        u.c = u.b;
                        f.b = ff(u.b/=2);
                        std::cerr << "u=" << u << ", f=" << f << std::endl;
                    }

                    const T lam   = u.c;
                    const T lam3  = lam*lam*lam;
                    const T rc    = f.c - f.a + sigma * u.c;
                    const T rb    = f.b - f.a + sigma * u.b;
                    const T beta  = (-lam/8 * rc + lam * rb) * 8/lam3;
                    const T gamma = (rc/4-rb) * 8/lam3;

                    ios::ocstream fp("zircon.dat");
                    for(T uu=0;uu<=u.c;uu+=T(0.001*u.c))
                    {
                        fp("%.15g %.15g %.15g %.15g %.15g\n",
                           double(uu),
                           double(ff(uu)),
                           double(f0-sigma*uu),
                           double(f.a-slope*uu),
                           double(f.a-sigma*uu + beta * uu*uu + gamma * uu *uu *uu));
                    }
                }


            }

            
            const array_type & probe(const T u) const throw();
            T                  objective(const array_type &Ftmp) throw(); //!< |Ftmp^2|/2

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(zircon);


            topology initialize(); //!< compute values from F and J



            template <typename FUNCTION>
            struct jwrap
            {
                jacobian<T> &jac;
                FUNCTION    &fcn;
                inline void operator()(matrix<T> &JJ, const readable<T> &XX) { jac(JJ,fcn,XX); }
            };

            template <typename FUNCTION>
            struct fwrap
            {
                FUNCTION         &func;
                zircon           &self;
                inline T operator()(const T u)
                {
                    func(self.FF,self.probe(u));
                    return self.objective(self.FF);
                }
            };


        };

    }

}

#endif
