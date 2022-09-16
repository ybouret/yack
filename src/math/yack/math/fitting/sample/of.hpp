//!\ file

#ifndef YACK_FIT_SAMPLE_OF_INCLUDED
#define YACK_FIT_SAMPLE_OF_INCLUDED 1

#include "yack/math/fitting/sample.hpp"
#include "yack/sort/indexing.hpp"
#include "yack/type/temporary.hpp"
#include "yack/field/in2d.hpp"

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
                typedef sample_of<ABSCISSA,ORDINATE>          self_type;       //!< alias
                typedef ark_ptr<string,self_type>             pointer;         //!< alias
                typedef sample<ABSCISSA,ORDINATE>             sample_type;     //!< alias
                typedef typename sample_type::comparator      comparator;      //!< alias
                typedef typename sample_type::allocator       allocator;       //!< alias
                typedef vector<size_t,allocator>              indices;         //!< alias
                typedef typename sample_type::sequential_type sequential_type; //!< alias
                typedef typename sample_type::ordinates       ordinates;       //!< alias
                using sample_type::xadd;                                       //!< alias
                using sample_type::curv;                                       //!< alias
                using sample_type::beta;                                       //!< alias
                using sample_type::prolog;
                using sample_type::epilog;
                using sample_type::z_diag;

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________

                //! setup with coherent fields
                template <typename ID>
                inline explicit sample_of(const ID                 &id,
                                          const readable<ABSCISSA> &X_,
                                          const readable<ORDINATE> &Y_,
                                          writable<ORDINATE>       &Z_) :
                sample_type(id),
                abscissa(X_),
                ordinate(Y_),
                adjusted(Z_),
                schedule(),
                deltaOrd(),
                dFda()
                {
                    assert(ordinate.size()==abscissa.size());
                    assert(adjusted.size()==abscissa.size());
                    const size_t n = abscissa.size();
                    schedule.adjust(n,0);
                    for(size_t i=n;i>0;--i) schedule[i] = i;
                    deltaOrd.adjust(n,0);
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
                    assert(ordinate.size()==abscissa.size());
                    assert(adjusted.size()==abscissa.size());
                    return abscissa.size();
                }

                //! make index by comparator
                virtual void make_indx(comparator cmp)
                {

                    const size_t n = dimension();
                    schedule.adjust(n,0);
                    if(cmp)
                    {
                        indexing::make(schedule,cmp,abscissa);
                    }
                    else
                    {
                        for(size_t i=n;i>0;--i) schedule[i] = i;
                    }
                }

                //! compute sequential D2
                virtual ORDINATE D2(sequential_type          &func,
                                    const readable<ORDINATE> &aorg)
                {
                    assert( dimension() == schedule.size() );
                    const size_t n = dimension();
                    if(n>0)
                    {
                        const variables &vars = **this;
                        xadd.resume( n );
                        {
                            const size_t ii = schedule[1];
                            xadd += squared( ordinate[ii] - (adjusted[ii] = func.start(abscissa[ii],aorg,vars))  );
                        }
                        for(size_t i=2;i<=n;++i)
                        {
                            const size_t ii = schedule[i];
                            xadd += squared( ordinate[ii] - (adjusted[ii] = func.reach(abscissa[ii],aorg,vars))  );
                        }
                        return xadd.get()/2;
                    }
                    else
                    {
                        return 0;
                    }
                }

            private:

                //! wrapper to compute derivative of a sequential function
                /**
                 df/da(x,a,v)
                 */
                struct callF
                {
                    sequential_type          &f;
                    ABSCISSA                  x;
                    const readable<ORDINATE> &a;
                    size_t                    i;
                    const variables          &v;

                    inline ORDINATE operator()(ORDINATE b)
                    {
                        assert(i>0);
                        assert(i<=a.size());
                        const temporary<ORDINATE> keep( coerce(a[i]), b);
                        return f.start(x,a,v);
                    }

                };
                
            public:

                //! compute sequential D2
                virtual ORDINATE D2_full(sequential_type            &func,
                                         const readable<ORDINATE>   &aorg,
                                         const readable<bool>       &used,
                                         const readable<ORDINATE>   &scal,
                                         derivative<ORDINATE>       &drvs)
                {
                    assert( dimension() == schedule.size() );
                    const variables &vars = **this;
                    const size_t     dims = dimension();
                    const size_t     nvar = vars.upper();
                    prolog(nvar);
                    if(dims>0)
                    {

                        //------------------------------------------------------
                        // pass 1 : evaluate adjusted, store deltaOrd and D2
                        //------------------------------------------------------
                        xadd.resume( dims );
                        deltaOrd.adjust(dims,0);
                        {
                            const size_t ii = schedule[1];
                            xadd += squared( deltaOrd[ii] = ordinate[ii] - (adjusted[ii] = func.start(abscissa[ii],aorg,vars)) );
                        }
                        for(size_t i=2;i<=dims;++i)
                        {
                            const size_t ii = schedule[i];
                            xadd += squared( deltaOrd[ii] = ordinate[ii] - (adjusted[ii] = func.start(abscissa[ii],aorg,vars)) );
                        }
                        const ORDINATE res = xadd.get()/2;

                        //------------------------------------------------------
                        // pass 2: cumulative
                        //------------------------------------------------------
                        dFda.adjust(nvar,0); // local memory
                        z_diag(used);        // zero used diagonal terms of curv

                        const coord2D     lo(1,1);
                        const coord2D     up(dims,nvar);
                        const layout2D    L(lo,up);
                        field2D<ORDINATE> B("beta",L);

                        for(size_t k=dims;k>0;--k)
                        {
                            // focus on abscissa[k]
                            callF          F = { func, abscissa[k], aorg, 0, vars };
                            const ORDINATE d = deltaOrd[k];

                            // compute gradient
                            dFda.ld(0);
                            for(const vnode *I=vars.head();I;I=I->next)
                            {
                                const size_t   i = F.i = ****I;if(!used[i]) continue;
                                const ORDINATE b = d * ( dFda[i] = drvs.diff(F,aorg[i],scal[i]) );
                                B[i][k]  = b;
                            }

                            // compute curvature
                            for(const vnode *I=vars.head();I;I=I->next)
                            {
                                const size_t        i      = ****I;if(!used[i]) continue;
                                writable<ORDINATE> &curv_i = curv[i];
                                const ORDINATE      dFda_i = dFda[i];
                                curv_i[i] += squared( dFda_i );
                                for(const vnode *J=I->next;J;J=J->next)
                                {
                                    const size_t j = ****J; if(!used[j]) continue;
                                    curv_i[j] += dFda_i * dFda[j];
                                }
                            }
                        }

                        
                        for(const vnode *I=vars.head();I;I=I->next)
                        {
                            const size_t i = ****I; if(!used[i]) continue;
                            beta[i]  = xadd.tableau( &B[i][1], dims);
                        }

                        //------------------------------------------------------
                        // pass 3: epilog
                        //------------------------------------------------------
                        epilog(nvar);
                        return res;
                    }
                    else
                    {
                        return 0;
                    }
                    
                }
                
                

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const readable<ABSCISSA> &abscissa;    //!< abscissae
                const readable<ORDINATE> &ordinate;    //!< ordinates
                writable<ORDINATE>       &adjusted;    //!< adjusted values
                indices                   schedule;    //!< sequential indexing
                ordinates                 deltaOrd;    //!< delta ordinate
                ordinates                 dFda;        //!< local gradient

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(sample_of);
            };

        }

    }

}

#endif
