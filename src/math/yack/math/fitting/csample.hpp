//!\ file

#ifndef YACK_FIT_CSAMPLE_INCLUDED
#define YACK_FIT_CSAMPLE_INCLUDED 1

#include "yack/math/fitting/sample-of.hpp"

namespace yack
{
    namespace math
    {

        namespace fitting
        {
            //__________________________________________________________________
            //
            //
            //! memory for sample of C-style data
            //
            //__________________________________________________________________
            
            template <
            typename ABSCISSA,
            typename ORDINATE>
            class csample_
            {
            public:
                //! cleanup
                inline virtual ~csample_() throw() { }
                
            protected:
                //! setup and load data
                explicit csample_(const ABSCISSA *x,
                                  const ORDINATE *y,
                                  const size_t    n) :
                X_(n),
                Y_(n),
                Z_(n)
                {
                    assert(yack_good(x,n));
                    assert(yack_good(y,n));
                    for(size_t i=0,j=i+1;i<n;++i,++j)
                    {
                        X_[j] = x[i];
                        Y_[j] = y[i];
                    }
                }

                
                vector<ABSCISSA,sample_::allocator> X_; //!< abscissae
                vector<ORDINATE,sample_::allocator> Y_; //!< ordinates
                vector<ORDINATE,sample_::allocator> Z_; //!< ajdusted


            private:
                YACK_DISABLE_COPY_AND_ASSIGN(csample_);
            };

            //__________________________________________________________________
            //
            //
            //!  sample of C-style data
            //
            //__________________________________________________________________
            template <
            typename ABSCISSA,
            typename ORDINATE>
            class csample : public csample_<ABSCISSA,ORDINATE>, public sample_of<ABSCISSA,ORDINATE>
            {
            public:
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________

                //! cleanup
                inline virtual ~csample() throw() {}

                
                //! setup, load and link data
                template <typename ID>
                inline explicit csample(const ID       &id,
                                        const ABSCISSA *x,
                                        const ORDINATE *y,
                                        const size_t    n) :
                csample_<ABSCISSA,ORDINATE>(x,y,n),
                sample_of<ABSCISSA,ORDINATE>(id,this->X_,this->Y_,this->Z_)
                {

                }

                

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(csample);
            };

        }

    }

}

#endif

