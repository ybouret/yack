//!\ file

#ifndef YACK_FIT_SAMPLES_OF_INCLUDED
#define YACK_FIT_SAMPLES_OF_INCLUDED 1

#include "yack/math/fitting/sample/c.hpp"
#include "yack/math/iota.hpp"

namespace yack
{
    namespace math
    {

        namespace fitting
        {
            //__________________________________________________________________
            //
            //
            //! set of samples
            //
            //__________________________________________________________________
            template <
            typename ABSCISSA,
            typename ORDINATE>
            class samples_of : public sample<ABSCISSA,ORDINATE>
            {
            public:
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________
                typedef sample<ABSCISSA,ORDINATE>             sample_type;      //!< alias
                typedef sample_of<ABSCISSA,ORDINATE>          single_type;      //!< alias
                typedef typename single_type::pointer         single_ptr;       //!< alias
                typedef suffix_set<string,single_ptr>         single_set;       //!< alias
                typedef typename single_set::knot_type        s_node;           //!< alias
                typedef typename sample_type::comparator      comparator;       //!< alias
                typedef typename sample_type::sequential_type sequential_type;  //!< alias
                using sample_type::xadd;                                        //!< alias
                using sample_type::curv;                                        //!< alias
                using sample_type::beta;                                        //!< alias
                using sample_type::prolog;
                using sample_type::epilog;
                using sample_type::z_diag;

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________

                //! setup empty
                template <typename ID>
                inline explicit samples_of(const ID &id) : sample_type(id), samples()
                {
                }

                //! cleanup
                inline virtual ~samples_of() throw()
                {
                }

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________

                //! register a new sample
                inline single_type & operator()(const string             &id,
                                                const readable<ABSCISSA> &X_,
                                                const readable<ORDINATE> &Y_,
                                                writable<ORDINATE>       &Z_)
                {
                    return create( new single_type(id,X_,Y_,Z_) );
                }

                //! register a new sample
                inline single_type & operator()(const char               *id,
                                                const readable<ABSCISSA> &X_,
                                                const readable<ORDINATE> &Y_,
                                                writable<ORDINATE>       &Z_)
                {
                    const string _(id); return (*this)(_,X_,Y_,Z_);
                }

                //! register a new sample from C-style data
                template <typename ID> inline
                single_type & operator()(const ID       &id,
                                         const ABSCISSA *x,
                                         const ORDINATE *y,
                                         const size_t    n)
                {
                    return create( new csample<ABSCISSA,ORDINATE>(id,x,y,n) );
                }
                
                
                //! fetch sample by name
                template <typename ID> inline
                const single_type & operator[](const ID &id) const
                {
                    return fetch(id);
                }

                //! fetch sample by name
                template <typename ID> inline
                single_type & operator[](const ID &id)
                {
                    return coerce(fetch(id));
                }


                //______________________________________________________________
                //
                // fast access
                //______________________________________________________________
                inline size_t        size() const throw() { return (*samples.tree).size; } //!< number of samples
                inline const s_node *head() const throw() { return (*samples.tree).head; } //!< fast iterator
                inline const s_node *tail() const throw() { return (*samples.tree).tail; } //!< fast iterator

                //______________________________________________________________
                //
                // interface
                //______________________________________________________________

                //! sum of all dimensions
                virtual size_t dimension() const throw()
                {
                    size_t res = 0;
                    for(const s_node *node=head();node;node=node->next)
                    {
                        const single_type &s = ***node;
                        res += s.dimension();
                    }
                    return res;
                }

                //! make all indices
                virtual void make_indx(comparator cmp)
                {
                    for(const s_node *node=head();node;node=node->next)
                    {
                        coerce(***node).make_indx(cmp);
                    }
                }

                //! weighted D2
                virtual ORDINATE D2(sequential_type          &func,
                                    const readable<ORDINATE> &aorg)
                {
                    xadd.resume(samples.size());
                    size_t total = 0;
                    for(const s_node *node=head();node;node=node->next)
                    {
                        single_type &s = coerce(***node);
                        const size_t n = s.dimension();
                        xadd  += s.D2(func,aorg)*n;
                        total += n;
                    }
                    return (total>0) ? xadd.get() /  total : 0;
                }

                virtual ORDINATE D2_full(sequential_type            &func,
                                         const readable<ORDINATE>   &aorg,
                                         const readable<bool>       &used,
                                         const readable<ORDINATE>   &scal,
                                         derivative<ORDINATE>       &drvs)
                {
                    const size_t nsam  = samples.size();
                    const size_t nvar  = (**this).upper();
                    size_t       total = 0;
                    xadd.resume(nsam);
                    prolog(nvar);

                    //------------------------------------------------------
                    // pass 1: compute individual D2, beta, curv
                    //------------------------------------------------------
                    for(const s_node *node=head();node;node=node->next)
                    {
                        single_type &s = coerce(***node);
                        const size_t n = s.dimension();
                        xadd  += s.D2_full(func,aorg,used,scal,drvs)*n;
                        total += n;
                    }
                    
                    if(total>0)
                    {
                        //------------------------------------------------------
                        // pass 2: combine beta and curv
                        //------------------------------------------------------
                        z_diag(used);
                        for(const s_node *node=head();node;node=node->next)
                        {
                            const single_type &s =  ***node;
                            const size_t       n = s.dimension();
                            for(const vnode *I=(*s).head();I;I=I->next)
                            {
                                const size_t i = ****I; if(!used[i]) continue;
                                beta[i] += n * s.beta[i];
                                for(const vnode *J=I;J;J=J->next)
                                {
                                    const size_t j = ****J; if(!used[j]) continue;
                                    curv[i][j] += n * s.curv[i][j];
                                }
                            }
                        }

                        //------------------------------------------------------
                        // pass 3: finalize
                        //------------------------------------------------------
                        iota::div_by(total,beta);
                        for(size_t i=nvar;i>0;--i)
                        {
                            for(size_t j=nvar;j>=i;--j) curv[i][j] /= total;
                        }

                        epilog(nvar);
                        return xadd.get() /  total;
                    }
                    else
                    {
                        return 0;
                    }
                }
                
                inline virtual void release() throw()
                {
                    for(const s_node *node=tail();node;node=node->prev)
                    {
                        single_type &s = coerce(***node);
                        s.release();
                    }
                    this->cleanup();
                }

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(samples_of);
                single_set     samples;
                
                inline const single_type &fetch(const string &id) const
                {
                    const single_ptr *pps = samples.search(id);
                    if(!pps) this->throw_unknown_name(id);
                    return **pps;
                }

                inline const single_type &fetch(const char *id) const
                {
                    const string _(id); return fetch(_);
                }

                inline single_type & create(single_type *ptr)
                {
                    single_ptr p = ptr;
                    if(!samples.insert(p)) this->throw_multiple_name(p->name);
                    return *p;
                }

            };
            
        }

    }

}

#endif
