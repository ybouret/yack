//! \file

#ifndef YACK_FIT_SAMPLES_INCLUDED
#define YACK_FIT_SAMPLES_INCLUDED 1

#include "yack/math/fit/sample.hpp"
#include "yack/associative/suffix/set.hpp"

namespace yack
{
    namespace math
    {

        namespace fit
        {

            //__________________________________________________________________
            //
            //
            //! set of single samples
            //
            //__________________________________________________________________
            template <
            typename ABSCISSA,
            typename ORDINATE>
            class samples_of :
            public sample<ABSCISSA,ORDINATE>,
            public suffix_set<string,ark_ptr<string,sample_of<ABSCISSA,ORDINATE> > >
            {
            public:
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________
                typedef sample<ABSCISSA,ORDINATE>             sample_type;      //!< alias
                typedef sample_of<ABSCISSA,ORDINATE>          single_type;      //!< alias
                typedef ark_ptr<string,single_type>           shared_type;      //!< alias
                typedef suffix_set<string,shared_type>        samples_set;      //!< alias
                typedef typename samples_set::const_iterator  const_iterator;   //!< alias
                typedef typename sample_type::sequential_type sequential_type;  //!< alias
                typedef typename sample_type::sequential_grad sequential_grad;  //!< alias

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________

                //! cleanup
                inline virtual ~samples_of() throw() {}

                //! setup with name
                template <typename ID> inline
                explicit samples_of(const ID &id) :
                sample_type(id),
                samples_set(),
                wksp()
                {
                }

                //______________________________________________________________
                //
                // sample interface
                //______________________________________________________________

                //! sum of dimensions
                inline virtual size_t dimension() const throw()
                {
                    size_t dims = 0;
                    for(const_iterator it=this->begin();it!=this->end();++it)
                    {
                        const single_type &s = **it;
                        dims += s.dimension();
                    }
                    return dims;
                }

                //! prepare all
                inline virtual void get_ready()
                {
                    for(const_iterator it=this->begin();it!=this->end();++it)
                    {
                        sample_type &s = coerce(**it);
                        s.get_ready();
                    }
                }

                //! weighted D2
                inline virtual ORDINATE D2(sequential_type          &func,
                                           const readable<ORDINATE> &aorg)
                {
                    wksp.free();
                    const size_t dims = dimension();
                    for(const_iterator it=this->begin();it!=this->end();++it)
                    {
                        single_type   &s = coerce(**it);
                        const ORDINATE n = static_cast<ORDINATE>( s.dimension() );
                        wksp.push_back( (n/dims) * s.D2(func,aorg) );
                    }
                    return sorted::sum(wksp,sorted::by_value);
                }


            private:
                YACK_DISABLE_COPY_AND_ASSIGN(samples_of);
                vector<ORDINATE> wksp;


            };

        }

    }

}

#endif
