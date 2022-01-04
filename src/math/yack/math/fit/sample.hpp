//! \file

#ifndef YACK_FIT_SAMPLE_INCLUDED
#define YACK_FIT_SAMPLE_INCLUDED 1

#include "yack/math/fit/sequential.hpp"
#include "yack/container/matrix.hpp"
#include "yack/type/utils.hpp"
#include "yack/ptr/auto.hpp"
#include "yack/sequence/vector.hpp"
#include "yack/sort/sum.hpp"
#include "yack/thing.hpp"

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
            class sample_ : public thing, public counted
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
                variables      vars; //!< variables
                
            protected:
                //! setup with name
                template <typename ID> inline
                explicit sample_(const ID &id) :
                thing(),
                counted(),
                name(id),
                vars()
                {}

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(sample_);

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
                typedef sequential<ABSCISSA,ORDINATE>      sequential_type; //!< alias
                typedef typename sequential_type::gradient sequential_grad; //!< alias

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
                virtual void     get_ready()                = 0; //!< prepare internal data

                //! compute D2 from parameters aorg using internal variables
                virtual ORDINATE D2(sequential_type          &func,
                                    const readable<ORDINATE> &aorg) = 0;


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


            private:
                YACK_DISABLE_COPY_AND_ASSIGN(sample);

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
            class sample_of : public sample<ABSCISSA,ORDINATE>
            {
            public:
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________
                typedef sample<ABSCISSA,ORDINATE>             sample_type;      //!< alias
                typedef typename sample_type::sequential_type sequential_type;  //!< alias
                typedef typename sample_type::sequential_grad sequential_grad;  //!< alias
                using sample_::vars;

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                //! setup
                template <typename ID> inline
                explicit sample_of(const ID                 &id,
                                   const readable<ABSCISSA> &x,
                                   const readable<ORDINATE> &y,
                                   writable<ORDINATE>       &z) :
                sample_type(id),
                abscissa(x),
                ordinate(y),
                adjusted(z),
                wksp(),
                indx(),
                beta(),
                dFda()
                {
                }

                //! cleanup
                inline virtual ~sample_of() throw()
                {
                }

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________

                //! common dimension
                inline virtual size_t dimension() const throw()
                {
                    assert(abscissa.size()==ordinate.size());
                    assert(adjusted.size()==ordinate.size());
                    assert(abscissa.size()==indx.size());
                    return abscissa.size();
                }

                //! prepare index and workspace
                inline virtual void get_ready()
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
                inline virtual ORDINATE D2(sequential_type          &F,
                                           const readable<ORDINATE> &A)
                {
                    assert(abscissa.size()==ordinate.size());
                    assert(adjusted.size()==ordinate.size());
                    assert(abscissa.size()==indx.size());
                    assert(abscissa.size()==wksp.size());

                    const size_t dims = dimension();
                    if(dims>0)
                    {
                        {
                            const size_t i = indx[1];
                            wksp[1] = squared(ordinate[i] - (adjusted[i] = F.start(abscissa[i],A,vars)));
                        }
                        for(size_t j=2;j<=dims;++j)
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



#if 0
                inline ORDINATE D2(sequential_type          &F,
                                   sequential_grad          &G,
                                   const readable<ORDINATE> &aorg,
                                   const readable<bool>     &used)
                {
                    assert(abscissa.size()==ordinate.size());
                    assert(adjusted.size()==ordinate.size());
                    assert(abscissa.size()==indx.size());
                    assert(abscissa.size()==wksp.size());

                    const size_t     npar = aorg.size();
                    const size_t     nvar = vars.size();
                    const size_t     dims = dimension();
                    dFda.adjust(nvar,0);
                    beta.adjust(npar,0);
                    beta.ld(0);
                    curv.ld(0);
                    if(dims>0)
                    {
                        //------------------------------------------------------
                        //
                        // first pass: store dY in workspace
                        //
                        //------------------------------------------------------
                        {
                            const size_t   i  = indx[1];
                            wksp[1] = (ordinate[i] - (adjusted[i] = F.start(abscissa[i],aorg,vars)));
                        }
                        for(size_t j=2;j<=dims;++j)
                        {
                            const size_t i = indx[j];
                            wksp[j] = ( ordinate[i] - (adjusted[i] = F.reach(abscissa[i],aorg,vars)));
                        }

                        //------------------------------------------------------
                        //
                        // second pass: accumulate gradient, curvature, and dY^2
                        //
                        //------------------------------------------------------
                        for(size_t j=1;j<=dims;++j)
                        {
                            dFda.ld(0);
                            const ORDINATE  dY  = square_pop(wksp[j]);
                            G(dFda,abscissa[ indx[j] ],aorg,vars,used);
                            for(size_t k=nvar;k>0;--k)
                            {
                                const size_t        kk      = *vars[k];
                                const ORDINATE      dFda_k  = dFda[k];
                                writable<ORDINATE> &curv_kk = curv[kk];
                                beta[kk] += dY * dFda[k];
                                for(size_t l=k;l>0;--l)
                                {
                                    const size_t ll = *vars[l];
                                    curv_kk[ll] += dFda_k * dFda[l];
                                }
                            }
                        }

                        return sorted::sum(wksp,sorted::by_value);
                    }
                    else
                    {
                        // clean
                        return 0;
                    }
                }

#endif






                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const readable<ABSCISSA> &abscissa; //!< abscissae
                const readable<ORDINATE> &ordinate; //!< ordinates
                writable<ORDINATE>       &adjusted; //!< adjusted ordinates
                vector<ORDINATE>          wksp;     //!< workspace
                vector<size_t>            indx;     //!< indexing abscissae
                vector<ORDINATE>          beta;     //!< descent direction
                vector<ORDINATE>          dFda;     //!< local gradients

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(sample_of);

            };


        }

    }

}

#endif
