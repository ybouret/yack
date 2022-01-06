//! \file

#ifndef YACK_FIT_LEAST_SQUARES_INCLUDED
#define YACK_FIT_LEAST_SQUARES_INCLUDED 1

#include "yack/math/fit/sample.hpp"
#include "yack/sequence/arrays.hpp"

namespace yack
{
    namespace math
    {
        namespace fit
        {

            template <
            typename ABSCISSA,
            typename ORDINATE>
            class least_squares :
            public large_object,
            public arrays_of<ORDINATE>
            {
            public:
                typedef sample<ABSCISSA,ORDINATE>                sample_type;
                typedef typename derivative<ORDINATE>::pointer   drvs_type;
                typedef arrays_of<ORDINATE>                      tableaux;
                typedef typename arrays_of<ORDINATE>::array_type array_type;

                inline virtual ~least_squares() throw() {}
                inline explicit least_squares() :
                large_object(),
                tableaux(4,0),
                step( this->next() ),
                curv(),
                drvs()
                {
                }



                array_type          &step; //!< current step
                matrix<ORDINATE>     curv; //!< modified matrix
                derivative<ORDINATE> drvs; //!< derivative

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(least_squares);

                void initialize(const size_t npar)
                {
                    this->make(npar);
                    curv.make(npar,npar);
                }

            };

        }
    }

}

#endif

