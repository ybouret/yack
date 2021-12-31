
//! \file

#ifndef YACK_FIT_SAMPLE_INCLUDED
#define YACK_FIT_SAMPLE_INCLUDED 1

#include "yack/math/fit/sequential.hpp"
#include "yack/type/gateway.hpp"
#include "yack/type/utils.hpp"
#include "yack/ptr/auto.hpp"
#include "yack/sequence/vector.hpp"
#include "yack/sort/sum.hpp"

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
                indx(),
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
                typedef sequential<ABSCISSA,ORDINATE> sequential_type; //!< alias
                using sample_::indx;

                //! create a wrapper for a simple function
                template <typename FUNC>
                class sequential_function : public sequential_type
                {
                public:
                    inline explicit sequential_function(FUNC &f) throw() : sequential_type(), host(f) {} //!< setup
                    inline virtual ~sequential_function()        throw() {}                              //!< cleanup

                private:
                    FUNC &host;
                    inline virtual ORDINATE on_start(const ABSCISSA ini, const readable<ORDINATE> &A, const variables &vars) { return host(ini,A,vars); }
                    inline virtual ORDINATE on_reach(const ABSCISSA, const ABSCISSA end, const readable<ORDINATE> &A, const variables &vars) { return host(end,A,vars); }
                };

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
                adjusted(z),
                wksp()
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
                    wksp.free();
                    for(size_t i=1;i<=n;++i)
                    {
                        indx.push_back(i);
                        wksp.push_back(0);
                    }

                }

                //! get D2(A) with sequential and parameters
                inline ORDINATE D2(sequential_type          &F,
                                   const readable<ORDINATE> &A)
                {
                    assert(abscissa.size()==ordinate.size());
                    assert(adjusted.size()==ordinate.size());
                    assert(abscissa.size()==indx.size());
                    assert(abscissa.size()==wksp.size());

                    const size_t size = abscissa.size();
                    if(size>0)
                    {
                        const variables &vars = **this;
                        {
                            const size_t i = indx[1];
                            wksp[1] = squared(ordinate[i] - (adjusted[i] = F.start(abscissa[i],A,vars)));
                        }
                        for(size_t j=2;j<=size;++j)
                        {
                            const size_t i = indx[j];
                            wksp[j] = squared( ordinate[i] - (adjusted[i] = F.reach(abscissa[i],A,vars)));
                        }
                        return sorted::sum(wksp,sorted::by_value);
                    }
                    else
                    {
                        return 0;
                    }
                }

                //! get D2(A) with FUNCTION and parameters
                template <typename FUNC>
                inline ORDINATE D2_(FUNC                     &f,
                                    const readable<ORDINATE> &A)
                {
                    sequential_function<FUNC> F(f);
                    return D2(F,A);
                }







                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const readable<ABSCISSA> &abscissa; //!< abscissae
                const readable<ORDINATE> &ordinate; //!< ordinates
                writable<ORDINATE>       &adjusted; //!< adjusted ordinates
                vector<ORDINATE>          wksp; //!< workspace

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(sample);
            };


        }

    }

}

#endif
