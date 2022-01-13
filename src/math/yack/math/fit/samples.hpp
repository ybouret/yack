//! \file

#ifndef YACK_FIT_SAMPLES_INCLUDED
#define YACK_FIT_SAMPLES_INCLUDED 1

#include "yack/math/fit/sample1.hpp"
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
                typedef typename sample_type::comparator      comparator;       //!< alias
                using sample_type::beta;
                using sample_type::curv;

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
                wksp(),
                temp()
                {
                }

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________

                //! insertion with multiple detection
                inline void use(const shared_type &ptr)
                {
                    if( ! this->insert(ptr) ) this->throw_multiple_name(ptr->name);
                }

                
                //! creation of a named, default sample
                template <typename ID>
                inline single_type & operator()(const ID                 &id,
                                                const readable<ABSCISSA> &x,
                                                const readable<ORDINATE> &y,
                                                writable<ORDINATE>       &z)
                {
                    shared_type  ptr = new single_type(id,x,y,z);
                    use(ptr);
                    return *ptr;
                }

                //! get sample by name
                single_type & operator[](const string &name)
                {
                    const shared_type *pps = this->search(name);
                    if(!pps) this->throw_unknown_name(name);
                    return coerce(**pps);
                }

                //! get sample by name, wrapper
                single_type & operator[](const char *name)
                {
                    const string _(name);
                    return (*this)[_];
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
                inline virtual void get_ready(comparator cmp)
                {
                    for(const_iterator it=this->begin();it!=this->end();++it)
                    {
                        sample_type &s = coerce(**it);
                        s.get_ready(cmp);
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

                //! full metrics for all the samples
                virtual ORDINATE D2_full(sequential_type          &func,
                                         const readable<ORDINATE> &aorg,
                                         const readable<bool>     &used,
                                         derivative<ORDINATE>     &drvs,
                                         const readable<ORDINATE> &scal)
                {

                    //----------------------------------------------------------
                    // prepare local curv and beta
                    //----------------------------------------------------------
                    this->initialize(aorg);

                    //----------------------------------------------------------
                    // first pass: collect D2 and build beta and curv
                    //----------------------------------------------------------
                    wksp.free();
                    const size_t dims = dimension();
                    for(const_iterator it=this->begin();it!=this->end();++it)
                    {
                        single_type   &s = coerce(**it);
                        const ORDINATE n = static_cast<ORDINATE>( s.dimension() );
                        wksp.push_back( (n/dims) * s.D2_full(func,aorg,used,drvs,scal) );
                    }

                    //----------------------------------------------------------
                    // second pass: build beta
                    //----------------------------------------------------------
                    const size_t n = aorg.size();
                    for(size_t k=n;k>0;--k)
                    {
                        if(!used[k]) continue;
                        temp.free();
                        for(const_iterator it=this->begin();it!=this->end();++it)
                        {
                            const single_type   &s = **it;
                            const ORDINATE       n = static_cast<ORDINATE>( s.dimension() );
                            const ORDINATE       w = (n/dims);
                            temp.push_back(w*s.beta[k]);
                        }
                        beta[k] = sorted::sum(temp,sorted::by_abs_value);
                    }

                    //----------------------------------------------------------
                    // third pass: build curv
                    //----------------------------------------------------------
                    for(size_t k=n;k>0;--k)
                    {
                        if(!used[k]) continue;
                        for(size_t l=k;l>0;--l)
                        {
                            if(!used[l]) continue;
                            temp.free();
                            for(const_iterator it=this->begin();it!=this->end();++it)
                            {
                                const single_type   &s = **it;
                                const ORDINATE       n = static_cast<ORDINATE>( s.dimension() );
                                const ORDINATE       w = (n/dims);
                                temp.push_back(w*s.curv[k][l]);
                            }
                            curv[k][l] = sorted::sum(temp,sorted::by_abs_value);
                        }
                    }

                    // and return D2
                    return sorted::sum(wksp,sorted::by_value);

                }


                //! update correlation
                virtual void update_correlation(correlation<ORDINATE> &corr) const
                {
                    for(const_iterator it=this->begin();it!=this->end();++it)
                    {
                        const single_type   &s = **it;
                        s.update_correlation(corr);
                    }
                }

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(samples_of);
                vector<ORDINATE> wksp;
                vector<ORDINATE> temp;

            };

        }

    }

}

#endif
