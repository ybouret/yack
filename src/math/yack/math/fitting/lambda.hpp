//! \file

#ifndef YACK_FITTING_LAMBDA_INCLUDED
#define YACK_FITTING_LAMBDA_INCLUDED 1

#include "yack/field/in1d.hpp"

namespace yack
{
    namespace math
    {

        namespace fitting
        {

            //__________________________________________________________________
            //
            //
            //! precomputed powers of ten for a given floating type
            //
            //__________________________________________________________________
            template <typename T>
            class lambda : public field1D<const T>
            {
            public:
                //______________________________________________________________
                //
                // types and definition
                //______________________________________________________________
                typedef field1D<const T> field_type; //!< alias
                static const char *clid() noexcept;   //!< lambda<T>
                static int         pmin() noexcept;   //!< ceil( log10( epsilon ) )
                static int         pmax() noexcept;   //!< MAX_10_EXP
                static int         ptol() noexcept;   //!< floor( log10( sqrt(epsilon) ) )
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                virtual ~lambda() noexcept; //!< cleanup
                explicit lambda();         //!< setup

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________
                T        initialized(int &p) const noexcept; //!< return self[p=pmin/2]
                void     initialize(int &p)  const noexcept; //!< p=pmin/2
                void     decrease(int &p)    const noexcept; //!< p = max_of(p-1,pmin)
                bool     increase(int &p)    const noexcept; //!< ++p if p<pmax, false otherwise

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(lambda);
            };

        }
    }
}

#endif

