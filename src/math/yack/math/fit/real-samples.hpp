
//! \file

#ifndef YACK_FIT_REAL_SAMPLES_INCLUDED
#define YACK_FIT_REAL_SAMPLES_INCLUDED 1

#include "yack/math/fit/samples.hpp"
#include "yack/math/fit/real-sample.hpp"

namespace yack
{
    namespace math
    {

        namespace fit
        {

            //__________________________________________________________________
            //
            //
            //! database of real samples
            //
            //__________________________________________________________________
            template <typename T>
            class real_samples_of : public samples_of<T,T>
            {
            public:
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                typedef samples_of<T,T>    samples_type;       //!< alias
                typedef real_sample_of<T>  real_sample_type;   //!< alias

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________

                //! cleanup
                virtual ~real_samples_of() throw();

                //! setup
                template <typename ID> inline
                explicit real_samples_of(const ID &id) :
                samples_type(id)
                {
                }

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________

                //! creation of a named real_sample
                template <typename ID>
                inline real_sample_type & operator()(const ID &id, const size_t n=0)
                {
                    return add( new real_sample_type(id,n) );
                }


            private:
                YACK_DISABLE_COPY_AND_ASSIGN(real_samples_of);
                real_sample_type &add(real_sample_type *);

            };

        }

    }

}

#endif
