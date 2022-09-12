//!\ file

#ifndef YACK_FIT_SAMPLES_OF_INCLUDED
#define YACK_FIT_SAMPLES_OF_INCLUDED 1

#include "yack/math/fitting/csample.hpp"

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
                typedef sample<ABSCISSA,ORDINATE>             sample_type; //!< alias
                typedef sample_of<ABSCISSA,ORDINATE>          single_type; //!< alias
                typedef typename single_type::pointer         single_ptr;  //!< alias
                typedef suffix_set<string,single_ptr>         single_set;  //!< alias
                typedef typename single_set::knot_type        s_node;      //!< alias
                typedef typename sample_type::comparator      comparator;  //!< alias
                typedef typename sample_type::sequential_type sequential_type;  //!< alias

                using sample_type::xadd;

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


                template <typename ID> inline
                const single_type & operator[](const ID &id) const
                {
                    return fetch(id);
                }

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


            private:
                YACK_DISABLE_COPY_AND_ASSIGN(samples_of);
                single_set samples;

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
