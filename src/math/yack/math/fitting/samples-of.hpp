//!\ file

#ifndef YACK_FIT_SAMPLES_OF_INCLUDED
#define YACK_FIT_SAMPLES_OF_INCLUDED 1

#include "yack/math/fitting/sample-of.hpp"

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
                typedef sample<ABSCISSA,ORDINATE>           sample_type; //!< alias
                typedef sample_of<ABSCISSA,ORDINATE>        single_type; //!< alias
                typedef typename single_type::pointer       single_ptr;  //!< alias
                typedef suffix_set<string,single_ptr>       single_set;  //!< alias
                typedef typename single_set::knot_type      s_node;      //!< alias
                typedef typename single_type:: abscissa     abscissa;    //!< alias
                typedef typename single_type:: ordinate     ordinate;    //!< alias
                typedef typename single_type:: adjusted     adjusted;    //!< alias

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
                inline single_type & operator()(const string   &id,
                                                const abscissa &X_,
                                                const ordinate &Y_,
                                                adjusted       &Z_)
                {
                    single_ptr p = new single_type(id,X_,Y_,Z_);
                    if(!samples.insert(p)) this->throw_multiple_name(id);
                    return *p;
                }

                //! register a new sample
                inline single_type & operator()(const char     *id,
                                                const abscissa &X_,
                                                const ordinate &Y_,
                                                adjusted       &Z_)
                {
                    const string _(id); return (*this)(_,X_,Y_,Z_);
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


            private:
                YACK_DISABLE_COPY_AND_ASSIGN(samples_of);
                single_set samples;
            };
            
        }

    }

}

#endif
