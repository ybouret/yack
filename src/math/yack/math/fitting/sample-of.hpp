
//!\ file

#ifndef YACK_FIT_SAMPLE_OF_INCLUDED
#define YACK_FIT_SAMPLE_OF_INCLUDED 1

#include "yack/math/fitting/sample.hpp"

namespace yack
{
    namespace math
    {

        namespace fitting
        {

            //__________________________________________________________________
            //
            //
            //! sample instance for one set of data
            //
            //__________________________________________________________________
            template <
            typename ABSCISSA,
            typename ORDINATE>
            class sample_of : public sample<ABSCISSA,ORDINATE>
            {
            public:
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________
                typedef sample_of<ABSCISSA,ORDINATE>     self_type;   //!< alias
                typedef ark_ptr<string,self_type>        pointer;     //!< alias
                typedef sample<ABSCISSA,ORDINATE>        sample_type; //!< alias
                typedef readable<ABSCISSA>               abscissa;    //!< alias
                typedef readable<ORDINATE>               ordinate;    //!< alias
                typedef writable<ORDINATE>               adjusted;    //!< alias
                typedef typename sample_type::comparator comparator;  //!< alias
                typedef typename sample_type::allocator  allocator;   //!< alias
                typedef vector<size_t,allocator>         indices;

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________

                //! setup with coherent fields
                template <typename ID>
                inline explicit sample_of(const ID       &id,
                                          const abscissa &X_,
                                          const ordinate &Y_,
                                          adjusted       &Z_) :
                sample_type(id),
                X(X_), Y(Y_), Z(Z_)
                {
                    assert(Y.size()==X.size());
                    assert(Z.size()==X.size());
                }

                //! cleanup
                inline virtual ~sample_of() throw()
                {
                }

                //______________________________________________________________
                //
                // interface
                //______________________________________________________________
                //! common size
                virtual size_t dimension() const throw()
                {
                    assert(Y.size()==X.size());
                    assert(Z.size()==X.size());
                    return X.size();
                }

                virtual void make_indx(comparator cmp)
                {
                    indx.adjust(dimension(),0);
                    
                }



                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const abscissa &X; //!< abscissae
                const ordinate &Y; //!< ordinates
                adjusted       &Z; //!< adjusted values

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(sample_of);
                indices indx;
                
            };

        }

    }

}

#endif
