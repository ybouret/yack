
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

            class sample_ : public object, public counted, public gateway<variables>
            {
            public:
                virtual       ~sample_() throw();
                const string & key() const throw();

                const string   name;
                vector<size_t> indx;

            protected:
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

            
            template <
            typename ABSCISSA,
            typename ORDINATE>
            class sample : public sample_
            {
            public:
                using sample_::indx;

                const readable<ABSCISSA> &abscissa;
                const readable<ORDINATE> &ordinate;
                writable<ORDINATE>       &adjusted;

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

                inline virtual ~sample() throw()
                {
                }

                inline size_t size() const throw()
                {
                    assert(abscissa.size()==ordinate.size());
                    assert(adjusted.size()==ordinate.size());
                    assert(abscissa.size()==indx.size());
                    return abscissa.size();
                }

                inline void setup()
                {
                    assert(abscissa.size()==ordinate.size());
                    assert(adjusted.size()==ordinate.size());

                    const size_t n = abscissa.size();
                    indx.free();
                    for(size_t i=1;i<=n;++i) indx.push_back(i);

                }

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


            private:
                YACK_DISABLE_COPY_AND_ASSIGN(sample);
            };


        }

    }

}

#endif
