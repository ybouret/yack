//! \file

#ifndef YACK_FIT_SAMPLE_INCLUDED
#define YACK_FIT_SAMPLE_INCLUDED 1

#include "yack/math/fit/sequential.hpp"
#include "yack/math/fcn/derivative.hpp"

#include "yack/type/utils.hpp"
#include "yack/type/gateway.hpp"
#include "yack/type/temporary.hpp"

#include "yack/sequence/vector.hpp"
#include "yack/sort/sum.hpp"
#include "yack/sort/indexing.hpp"

#include "yack/large-object.hpp"

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
            class sample_ : public large_object, public counted, public gateway<variables>
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

            protected:
                //! setup with name
                template <typename ID> inline
                explicit sample_(const ID &id) :
                large_object(),
                counted(),
                name(id),
                vars()
                {}

                //! throw if multiple names for samples
                void throw_multiple_name(const string &) const;

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(sample_);
                virtual const_type &bulk() const throw();
                variables vars; //!< variables

            };

            //__________________________________________________________________
            //
            //
            //! sample interface
            //
            //__________________________________________________________________
            template <
            typename ABSCISSA,
            typename ORDINATE>
            class sample : public sample_
            {
            public:
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________
                typedef sequential<ABSCISSA,ORDINATE>      sequential_type;    //!< alias
                typedef int (*comparator)(const ABSCISSA &, const ABSCISSA &); //!< alias


                //! create a wrapper for a simple function
                template <typename FUNC>
                class sequential_wrapper : public sequential_type
                {
                public:
                    inline explicit sequential_wrapper(FUNC &f) throw() : sequential_type(), host(f) {} //!< setup
                    inline virtual ~sequential_wrapper()        throw() {}                              //!< cleanup

                private:
                    YACK_DISABLE_COPY_AND_ASSIGN(sequential_wrapper);
                    FUNC &host;
                    inline virtual ORDINATE on_start(const ABSCISSA ini, const readable<ORDINATE> &A, const variables &vars) { return host(ini,A,vars); }
                    inline virtual ORDINATE on_reach(const ABSCISSA, const ABSCISSA end, const readable<ORDINATE> &A, const variables &vars) { return host(end,A,vars); }
                };

                //______________________________________________________________
                //
                // virtual interface
                //______________________________________________________________
                virtual size_t   dimension() const throw()  = 0; //!< number of data
                virtual void     get_ready(comparator)      = 0; //!< prepare internal data

                //! compute D2 from parameters aorg
                virtual ORDINATE D2(sequential_type          &func,
                                    const readable<ORDINATE> &aorg) = 0;

                //! compute D2 and full metrics from aorg and used
                virtual ORDINATE D2_full(sequential_type          &F,
                                         const readable<ORDINATE> &aorg,
                                         const readable<bool>     &used,
                                         derivative<ORDINATE>     &drvs,
                                         const readable<ORDINATE> &scal) = 0;

                //______________________________________________________________
                //
                // non virtual interface
                //______________________________________________________________

                //! helper for regular functions
                template <typename FUNC> inline
                ORDINATE D2_(FUNC                     &func,
                             const readable<ORDINATE> &aorg)
                {
                    sequential_wrapper<FUNC> call(func);
                    return D2(call,aorg);
                }


                //! helper for regular functions
                template <typename FUNC>
                inline ORDINATE D2_full_(FUNC                    &func,
                                         const readable<ORDINATE> &aorg,
                                         const readable<bool>     &used,
                                         derivative<ORDINATE>     &drvs,
                                         const readable<ORDINATE> &scal)
                {
                    sequential_wrapper<FUNC> call(func);
                    return D2_full(call,aorg,used,drvs,scal);
                }

                //! make symmetric curv
                void finalize() throw()
                {
                    assert(curv.is_square());
                    const size_t n = curv.rows;
                    for(size_t k=n;k>0;--k)
                    {
                        for(size_t l=k-1;l>0;--l)
                        {
                            curv[l][k] = curv[k][l];
                        }
                    }
                }

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                matrix<ORDINATE> curv; //!< curvature matrix
                vector<ORDINATE> beta; //!< descent direction

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________

                //! cleanup
                inline virtual ~sample() throw() {}

            protected:

                //! setup with name
                template <typename ID> inline
                explicit sample(const ID &id) :
                sample_(id),
                curv(),
                beta()
                {
                }

                //! helper for D2_full
                inline void make(const collection &params)
                {
                    const size_t npar = params.size();
                    curv.make(npar,npar);
                    beta.adjust(npar,0);

                    curv.ld(0);
                    beta.ld(0);
                }


            private:
                YACK_DISABLE_COPY_AND_ASSIGN(sample);

            };



          

        }

    }

}

#endif
