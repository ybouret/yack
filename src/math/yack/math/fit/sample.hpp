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
            class sample_ : public thing, public counted, public gateway<variables>
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
                thing(),
                counted(),
                name(id),
                vars()
                {}

                //! throw if multiple names for samples
                void throw_multiple_name(const string &) const;

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(sample_);
                virtual const_type &bulk() const throw();
                variables      vars; //!< variables

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
                typedef typename sample_type::comparator      comparator;       //!< alias
                typedef variables::const_iterator             var_iterator;     //!< alias
                using sample_type::beta;
                using sample_type::curv;

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
                inline virtual void get_ready(comparator cmp)
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

                    if(cmp)
                    {
                        indexing::make(indx,*cmp,abscissa);
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
                            wksp[1] = squared(ordinate[i] - (adjusted[i] = F.start(abscissa[i],A,**this)));
                        }
                        for(size_t j=2;j<=dims;++j)
                        {
                            const size_t i = indx[j];
                            wksp[j] = squared( ordinate[i] - (adjusted[i] = F.reach(abscissa[i],A,**this)));
                        }
                        return sorted::sum(wksp,sorted::by_value)/2;
                    }
                    else
                    {
                        return 0;
                    }
                }
                

                inline virtual ORDINATE D2_full(sequential_type          &F,
                                                const readable<ORDINATE> &aorg,
                                                const readable<bool>     &used,
                                                derivative<ORDINATE>     &drvs,
                                                const readable<ORDINATE> &scal)
                {
                    assert(abscissa.size()==ordinate.size());
                    assert(adjusted.size()==ordinate.size());
                    assert(abscissa.size()==indx.size());
                    assert(abscissa.size()==wksp.size());
                    assert(aorg.size()==used.size());

                    const variables &vars = **this;        assert(vars.span()<=aorg.size());
                    const size_t     npar = aorg.size();
                    const size_t     nvar = vars.size();
                    const size_t     dims = dimension();
                    this->make(aorg);
                    dFda.adjust(npar,0);


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
                        var_iterator first_var = vars.begin();
                        for(size_t j=1;j<=dims;++j)
                        {
                            //--------------------------------------------------
                            // get dY and square wksp[j]
                            //--------------------------------------------------
                            const ORDINATE            dY = square_pop(wksp[j]);

                            //--------------------------------------------------
                            // prepare 1D function for derivative
                            //--------------------------------------------------
                            call1D                    F1 = { 0, aorg, vars, F, abscissa[ indx[j] ] };

                            //--------------------------------------------------
                            // loop over variables to fill dFda
                            //--------------------------------------------------
                            dFda.ld(0);
                            {
                                var_iterator it = first_var;
                                for(size_t iv=nvar;iv>0;--iv,++it)
                                {
                                    const variable &var = **it;
                                    if( !var(used) ) continue;
                                    F1.apos   = *var;
                                    var(dFda) = drvs.diff(F1,var(aorg),var(scal));
                                }
                            }

                            //--------------------------------------------------
                            // update descent direction and curvature
                            // variables could have mixed-up indices,
                            // so use the full parameters range
                            //--------------------------------------------------
                            for(size_t k=npar;k>0;--k)
                            {
                                if(!used[k]) continue;
                                const ORDINATE      dFda_k  = dFda[k];
                                writable<ORDINATE> &curv_k  = curv[k];
                                beta[k] -= dY * dFda[k];
                                for(size_t l=k;l>0;--l)
                                {
                                    if(!used[l]) continue;
                                    curv_k[l] += dFda_k * dFda[l];
                                }
                            }
                            
                        }


                        return sorted::sum(wksp,sorted::by_value)/2;
                    }
                    else
                    {
                        // clean
                        return 0;
                    }
                }




                

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const readable<ABSCISSA> &abscissa; //!< abscissae
                const readable<ORDINATE> &ordinate; //!< ordinates
                writable<ORDINATE>       &adjusted; //!< adjusted ordinates
                vector<ORDINATE>          wksp;     //!< workspace
                vector<size_t>            indx;     //!< indexing abscissae
                vector<ORDINATE>          dFda;     //!< local gradients

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(sample_of);
                struct call1D
                {
                    size_t                    apos;
                    const readable<ORDINATE> &atmp;
                    const variables          &vars;
                    sequential_type          &func;
                    const ABSCISSA           &xpos;

                    inline ORDINATE operator()(const ORDINATE A)
                    {
                        ORDINATE                  &a = coerce(atmp[apos]);
                        const temporary<ORDINATE>  t(a,A);
                        return func.start(xpos,atmp,vars);
                    }
                };
            };


        }

    }

}

#endif
