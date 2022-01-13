

//! \file

#ifndef YACK_FIT_REAL_SAMPLE_INCLUDED
#define YACK_FIT_REAL_SAMPLE_INCLUDED 1

#include "yack/math/fit/sample1.hpp"
#include "yack/sequence/arrays.hpp"

namespace yack
{
    namespace math
    {

        namespace fit
        {

            //__________________________________________________________________
            //
            //
            //! sample real/real
            //
            //__________________________________________________________________
            template <typename T>
            class real_sample_of : public arrays_of<T>, public sample_of<T,T>
            {
            public:
                //______________________________________________________________
                //
                // types and defintions
                //______________________________________________________________
                typedef arrays_of<T> tableaux;                       //!< alias
                typedef sample_of<T,T>  sample_type;                 //!< alias
                static int compare(const T &, const T &) throw();    //!< for indexing

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________

                //! cleanup
                virtual ~real_sample_of() throw();

                //! setup
                template <typename NAME> inline
                explicit real_sample_of(const NAME &id, const size_t dim=0) :
                tableaux(3,dim),
                sample_type(id, (*this)[1], (*this)[2], (*this)[3] )
                {
                }

                


            private:
                YACK_DISABLE_COPY_AND_ASSIGN(real_sample_of);
            };

        }

    }

}

#endif
