
#ifndef YACK_FIT_GLS_INCLUDED
#define YACK_FIT_GLS_INCLUDED 1

#include "yack/math/fitting/least-squares.hpp"

namespace yack
{
    namespace math
    {

        namespace fitting
        {
            template <typename T> class gls : public least_squares<T,T>
            {
            public:
                typedef least_squares<T,T>          ls_type;
                typedef typename ls_type::drvs_ptr  drvs_ptr;


                explicit gls(const drvs_ptr *ppDrvs=NULL);
                virtual ~gls() throw();


            private:
                YACK_DISABLE_COPY_AND_ASSIGN(gls);
            };

        }

    }

}

#endif
