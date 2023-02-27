//!\ file

#ifndef YACK_FIT_SAMPLE_OF_INCLUDED
#define YACK_FIT_SAMPLE_OF_INCLUDED 1

#include "yack/math/fitting/sample.hpp"
#include "yack/sort/indexing.hpp"
#include "yack/type/temporary.hpp"

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
                dFda(),
                Beta()
                {
                    assert(ordinate.size()==abscissa.size());
                    assert(adjusted.size()==abscissa.size());
                    const size_t n = abscissa.size();
                    schedule.adjust(n,0);
                    for(size_t i=n;i>0;--i) schedule[i] = i;
                    deltaOrd.adjust(n,0);
                }
                
                //! cleanup
                inline virtual ~sample_of() noexcept
                {
                }
                
                //______________________________________________________________
                //
                // interface
                //______________________________________________________________
             
                //! common size
                virtual size_t dimension() const noexcept
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
                            const size_t    ii  = schedule[1];
                            const ORDINATE  o1 = ordinate[ii] - (adjusted[ii] = func.start(abscissa[ii],aorg,vars));
                            const ORDINATE  o2 = squared(o1 );
                            xadd.push_(o2);
                        }
                        for(size_t i=2;i<=n;++i)
                        {
                            const size_t    ii = schedule[i];
                            const ORDINATE  o1 = ordinate[ii] - (adjusted[ii] = func.reach(abscissa[ii],aorg,vars));
                            const ORDINATE  o2 = squared(o1);
                            xadd.push_(o2);
                        }
                        return xadd.sum()/2;
                    }
                    else
                    {
                        return 0;
                    }
                }

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
                            const size_t    ii = schedule[1];
                            const ORDINATE  o1 = ordinate[ii] - (adjusted[ii] = func.start(abscissa[ii],aorg,vars));
                            deltaOrd[ii] = o1;
                            const ORDINATE  o2 = squared(o1);
                            xadd.push_(o2);
                        }
                        for(size_t i=2;i<=dims;++i)
                        {
                            const size_t    ii = schedule[i];
                            const ORDINATE  o1 = ordinate[ii] - (adjusted[ii] = func.reach(abscissa[ii],aorg,vars));
                            deltaOrd[ii] = o1;
                            const ORDINATE  o2 = squared(o1);
                            xadd.push_(o2);
                        }
                        const ORDINATE res = xadd.sum()/2;
                        
                        //------------------------------------------------------
                        // pass 2: cumulative
                        //------------------------------------------------------
                        dFda.adjust(nvar,0); // local memory
                        z_diag(used);        // zero used diagonal terms of curv
                        if(nvar&&dims)
                            Beta.make(nvar,dims);
                        
                        
                        for(size_t k=dims;k>0;--k)
                        {
                            // focus on abscissa[k]
                            callF          F = { func, abscissa[k], aorg, 0, vars };
                            const ORDINATE d = deltaOrd[k];
                            
                            // compute local gradient and store for global gradient
                            dFda.ld(0);
                            for(const vnode *I=vars.head();I;I=I->next)
                            {
                                const size_t   i = F.i = ****I;if(!used[i]) continue;
                                const ORDINATE b = d * ( dFda[i] = drvs.diff(F,aorg[i],scal[i]) );
                                Beta[i][k]  = b;
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
                        
                        // compute global gradient
                        for(const vnode *I=vars.head();I;I=I->next)
                        {
                            const size_t i = ****I;
                            if(!used[i])
                            {
                                assert( std::abs(beta[i]) <= 0);
                                continue;
                            }
                            beta[i]  = xadd.tableau(Beta[i]);
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

                //! release all memory
                inline virtual void release() noexcept
                {
                    Beta.release();
                    dFda.release();
                    deltaOrd.release();
                    schedule.release();
                    this->cleanup();
                }

                //! provide info to correlation
                inline virtual void update(correlation<ORDINATE> &cr) const
                {
                    const size_t n = dimension();
                    for(size_t i=n;i>0;--i)
                    {
                        cr( ordinate[i], adjusted[i] );
                    }
                }

                //! append this to sequence
                inline virtual void     collect(sequence<sample_type *> &db) const
                {
                    const sample_type &self = *this;
                    db.push_back( &coerce(self) );
                }

            private:
                inline void save_to(ios::ostream &fp) const
                {
                    const size_t  nd = dimension();
                    for(size_t i=1;i<=nd;++i)
                    {
                        fp("%.15g %.15g %.15g\n",double(abscissa[i]),double(ordinate[i]),double(adjusted[i]));
                    }
                }
                
            public:
                //! save, converting to double
                inline void save() const
                {
                    const string  fn = this->name + ".dat";
                    ios::ocstream fp(fn);
                    save_to(fp);
                }
                
                //! save with suffix, converting to double
                template <typename SUFFIX> inline
                void save_with(const SUFFIX &suffix) const
                {
                    const string  fn = this->name + suffix + ".dat";
                    ios::ocstream fp(fn);
                    save_to(fp);
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
                matrix<ORDINATE>          Beta;        //!< local fields for Beta
                                                       
            private:
                YACK_DISABLE_COPY_AND_ASSIGN(sample_of);
                
            };
            
        }
        
    }
    
}

#endif
