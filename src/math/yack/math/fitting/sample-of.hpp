//!\ file

#ifndef YACK_FIT_SAMPLE_OF_INCLUDED
#define YACK_FIT_SAMPLE_OF_INCLUDED 1

#include "yack/math/fitting/sample.hpp"
#include "yack/sort/indexing.hpp"

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
                typedef typename sample_type::ordinates        ordinates;      //!< alias
                using sample_type::xadd;                                       //!< alias
                using sample_type::curv;                                       //!< alias
                using sample_type::beta;                                       //!< alias

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
                deltaOrd()
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
                            xadd += squared( (adjusted[ii] = func.start(abscissa[ii],aorg,vars)) - ordinate[ii] );
                        }
                        for(size_t i=2;i<=n;++i)
                        {
                            const size_t ii = schedule[i];
                            xadd += squared( (adjusted[ii] = func.reach(abscissa[ii],aorg,vars)) - ordinate[ii] );
                        }
                        return xadd.get()/2;
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
                                         const derivative<ORDINATE> &drvs)
                {
                    assert( dimension() == schedule.size() );
                    const variables &vars = **this;
                    const size_t     dims = dimension();
                    const size_t     nvar = vars.upper();
                    this->prepare(nvar);
                    if(dims>0)
                    {
                        // pass 1
                        xadd.resume( dims );
                        {
                            const size_t ii = schedule[1];
                            xadd += squared( (adjusted[ii] = func.start(abscissa[ii],aorg,vars))  - ordinate[ii] );
                        }
                        for(size_t i=2;i<=dims;++i)
                        {
                            const size_t ii = schedule[i];
                            xadd += squared(  (adjusted[ii] = func.reach(abscissa[ii],aorg,vars))  - ordinate[ii] );
                        }
                        const ORDINATE res = xadd.get()/2;
                        
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
                                                       
            private:
                YACK_DISABLE_COPY_AND_ASSIGN(sample_of);
            };

        }

    }

}

#endif
