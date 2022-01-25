
//! \file

#ifndef YACK_FIT_REAL_SAMPLES_INCLUDED
#define YACK_FIT_REAL_SAMPLES_INCLUDED 1

#include "yack/math/fit/samples.hpp"
#include "yack/math/fit/sample/real.hpp"

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

                //! creation from data
                template <typename ID, typename U, typename V>
                inline real_sample_type & operator()(const ID &id, const U x[], const V y[], const size_t n)
                {
                    assert( yack_good(x,n) );
                    assert( yack_good(y,n) );

                    //real_sample_type & s =   add( new real_sample_type(id,n) );
                    real_sample_type & s =  (*this)(id,n);

                    assert( s.mutual_size() == n);
                    for(size_t i=0,j=1;i<n;++i,++j)
                    {
                        coerce(s.abscissa[j]) = static_cast<T>(x[i]);
                        coerce(s.ordinate[j]) = static_cast<T>(y[i]);
                    }
                    s.prepare();
                    return s;
                }



            private:
                YACK_DISABLE_COPY_AND_ASSIGN(real_samples_of);
                real_sample_type &add(real_sample_type *);

            };

        }

    }

}

#endif
