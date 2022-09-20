
#ifndef YACK_FIT_GLS_INCLUDED
#define YACK_FIT_GLS_INCLUDED 1

#include "yack/math/fitting/least-squares.hpp"

namespace yack
{
    namespace math
    {

        namespace fitting
        {
            //__________________________________________________________________
            //
            //
            //! scalar General Least Squares
            //
            //__________________________________________________________________
            template <typename T> class gls : public least_squares<T,T>
            {
            public:
                //______________________________________________________________
                //
                // types and definition
                //______________________________________________________________
                typedef least_squares<T,T>          ls_type;   //!< alias
                typedef typename ls_type::drvs_type drvs_type; //!< alias
                typedef typename ls_type::drvs_ptr  drvs_ptr;  //!< alias

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                explicit gls(const drvs_ptr *ppDrvs=NULL); //!< setup
                virtual ~gls() throw();                    //!< cleanup


            private:
                YACK_DISABLE_COPY_AND_ASSIGN(gls);
            };

        }

    }

}

#endif
