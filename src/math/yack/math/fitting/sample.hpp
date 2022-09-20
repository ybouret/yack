//! \file

#ifndef YACK_FIT_SAMPLE_INCLUDED
#define YACK_FIT_SAMPLE_INCLUDED 1

#include "yack/math/fitting/sample/base.hpp"
#include "yack/math/fitting/sequential.hpp"
#include "yack/container/matrix.hpp"
#include "yack/sequence/vector.hpp"
#include "yack/math/adder.hpp"
#include "yack/math/fcn/derivative.hpp"
#include "yack/math/data/corr.hpp"

namespace yack
{
    namespace math
    {

        namespace fitting
        {
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
                typedef vector<ORDINATE,allocator>         ordinates;          //!< alias
                typedef adder<ORDINATE>                    adder_type;         //!< alias
                typedef derivative<ORDINATE>               drvs_type;          //!< alias
                typedef int (*comparator)(const ABSCISSA &, const ABSCISSA &); //!< alias to create schedule

                //______________________________________________________________
                //
                //! create a wrapper for a simple function
                //______________________________________________________________
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
                virtual size_t   dimension() const throw() = 0;          //!< number of data points.
                virtual void     make_indx(comparator)     = 0;          //!< indices for sequential evaluation

                //--------------------------------------------------------------
                //! compute D2
                //--------------------------------------------------------------
                virtual ORDINATE D2(sequential_type          &func,
                                    const readable<ORDINATE> &aorg) = 0;

                //--------------------------------------------------------------
                //! compute D2, gradient and curvature
                //--------------------------------------------------------------
                virtual ORDINATE D2_full(sequential_type            &func,
                                         const readable<ORDINATE>   &aorg,
                                         const readable<bool>       &used,
                                         const readable<ORDINATE>   &scal,
                                         derivative<ORDINATE>       &drvs) = 0;

                //--------------------------------------------------------------
                //! update correlation with ordinate/adjusted
                //--------------------------------------------------------------
                virtual void     update(correlation<ORDINATE> &cr) const = 0;

                //______________________________________________________________
                //
                // non virtual interface
                //______________________________________________________________

                //! compute correlation
                inline ORDINATE corr(correlation<ORDINATE> &cr) const
                {
                    cr.free();
                    update(cr);
                    return cr();
                }

                //______________________________________________________________
                //
                // helpers
                //______________________________________________________________
              
                //! wrapper to compute D2 for a regular function
                template <typename FUNC> inline
                ORDINATE D2_for(FUNC                     &func,
                                const readable<ORDINATE> &aorg )
                {
                    sequential_wrapper<FUNC> call(func);
                    return D2(call,aorg);
                }


                //! wrapper to compute D2_full for a regular function
                template <typename FUNC> inline
                ORDINATE D2_full_for(FUNC                       &func,
                                     const readable<ORDINATE>   &aorg,
                                     const readable<bool>       &used,
                                     const readable<ORDINATE>   &scal,
                                     derivative<ORDINATE>       &drvs)
                {
                    sequential_wrapper<FUNC> call(func);
                    return D2_full(call,aorg,used,scal,drvs);
                }



                //______________________________________________________________
                //
                // C++
                //______________________________________________________________

                //! cleanup
                inline virtual ~sample() throw() {}

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                matrix<ORDINATE> curv; //!< curvature matrix
                ordinates        beta; //!< gradient
                adder_type       xadd; //!< adder
                
            protected:
                //______________________________________________________________
                //
                //! setup with name
                //______________________________________________________________
                template <typename ID> inline
                explicit sample(const ID &id) :
                sample_(id),
                curv(),
                beta(),
                xadd()
                {
                }

                //______________________________________________________________
                //
                //! make a symmetric matrix
                //______________________________________________________________
                inline void epilog(const size_t nvar) throw()
                {
                    for(size_t i=2;i<=nvar;++i)
                    {
                        for(size_t j=1;j<i;++j)
                        {
                            curv[i][j] = curv[j][i];
                        }
                    }
                }

                //______________________________________________________________
                //
                //! set beta=0 and curv=Id
                //______________________________________________________________
                inline void prolog(const size_t nvar)
                {
                    curv.make(nvar,nvar);
                    beta.adjust(nvar,0);
                    curv.ld(0);
                    beta.ld(0);

                    for(size_t i=nvar;i>0;--i) curv[i][i] = 1;
                }

                //______________________________________________________________
                //
                //! releasing memory
                //______________________________________________________________
                inline void cleanup() throw()
                {
                    beta.release();
                    curv.release();
                }
                
                //______________________________________________________________
                //
                //! zero used curv diagonal points
                //______________________________________________________________
                inline void z_diag(const readable<bool> &used) throw()
                {
                    for(const vnode *I=(**this).head();I;I=I->next)
                    {
                        const size_t i = ****I;if(!used[i]) continue;
                        curv[i][i] = 0;
                    }
                }


            private:
                YACK_DISABLE_COPY_AND_ASSIGN(sample);

            };

        }
    }
}

#endif

