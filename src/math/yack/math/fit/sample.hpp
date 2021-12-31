
//! \file

#ifndef YACK_FIT_SAMPLE_INCLUDED
#define YACK_FIT_SAMPLE_INCLUDED 1

#include "yack/math/fit/variables.hpp"
#include "yack/type/gateway.hpp"
#include "yack/type/utils.hpp"
#include "yack/ptr/auto.hpp"
#include "yack/sequence/vector.hpp"

namespace yack
{
    namespace math
    {

        namespace fit
        {
            //__________________________________________________________________
            //
            //
            //! sample base type
            //
            //__________________________________________________________________
            class sample_ : public object, public counted, public gateway<variables>
            {
            public:
                //______________________________________________________________
                //
                // methods
                //______________________________________________________________
                virtual       ~sample_()   throw(); //!< cleanup
                const string & key() const throw(); //!< for db

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const string   name; //!< identifier
                vector<size_t> indx; //!< to access data

            protected:
                //! setup with name
                template <typename ID> inline
                explicit sample_(const ID &id) :
                object(),
                counted(),
                name(id),
                vars(new variables())
                {}

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(sample_);
                virtual const_type & bulk() const throw();
                auto_ptr<variables> vars;
            };

            //__________________________________________________________________
            //
            //
            //! generic sample
            //
            //__________________________________________________________________
            template <
            typename ABSCISSA,
            typename ORDINATE>
            class sample : public sample_
            {
            public:
                using sample_::indx;


                //______________________________________________________________
                //
                // methods
                //______________________________________________________________
                //! setup
                template <typename ID> inline
                explicit sample(const ID                 &id,
                                const readable<ABSCISSA> &x,
                                const readable<ORDINATE> &y,
                                writable<ORDINATE>       &z) :
                sample_(id),
                abscissa(x),
                ordinate(y),
                adjusted(z)
                {
                }

                //! cleanup
                inline virtual ~sample() throw()
                {
                }

                //! common size
                inline size_t size() const throw()
                {
                    assert(abscissa.size()==ordinate.size());
                    assert(adjusted.size()==ordinate.size());
                    assert(abscissa.size()==indx.size());
                    return abscissa.size();
                }

                //! prepare index
                inline void setup()
                {
                    assert(abscissa.size()==ordinate.size());
                    assert(adjusted.size()==ordinate.size());

                    const size_t n = abscissa.size();
                    indx.free();
                    for(size_t i=1;i<=n;++i) indx.push_back(i);

                }

                //! get D2(A) with function and parameters
                template <typename FUNC>
                ORDINATE D2(FUNC &F, const readable<ORDINATE> &A)
                {
                    assert(abscissa.size()==ordinate.size());
                    assert(adjusted.size()==ordinate.size());

                    ORDINATE         sum2 = 0;
                    const variables &vars = **this;
                    const size_t     size = abscissa.size();
                    for(size_t j=1;j<=size;++j)
                    {
                        const size_t i = indx[j];
                        sum2 += squared( ordinate[i] - (adjusted[i] = F(abscissa[i],A,vars)));
                    }
                    return sum2;
                }

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const readable<ABSCISSA> &abscissa; //!< abscissae
                const readable<ORDINATE> &ordinate; //!< ordinates
                writable<ORDINATE>       &adjusted; //!< adjusted ordinates

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(sample);
            };


        }

    }

}

#endif
