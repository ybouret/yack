//! \file

#ifndef YACK_FIT_VARIABLES_INCLUDED
#define YACK_FIT_VARIABLES_INCLUDED 1

#include "yack/math/fit/variable/replica.hpp"
#include "yack/math/data/percent.hpp"
#include "yack/associative/suffix/set.hpp"
#include "yack/ios/fmt/align.hpp"
#include "yack/type/utils.hpp"
#include "yack/string/tokenizer.hpp"

namespace yack
{
    namespace math
    {

        namespace fit
        {

            //__________________________________________________________________
            //
            //
            //! base class for variables database
            //
            //__________________________________________________________________
            typedef suffix_set<string,replica::handle> variables_;

            //__________________________________________________________________
            //
            //
            //! database of variables
            //
            //__________________________________________________________________
            class variables :  public object , public variables_
            {
            public:
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________
                typedef replica::handle handle; //!< alias
                typedef vector<string>  strings; //!< alias

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                explicit variables() throw();                //!< setup empty
                virtual ~variables() throw();                //!< cleanup
                variables(const variables &);                //!< copy
                variables & operator=(const variables &);    //!< assing

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________

                //! max name size
                size_t width() const throw();

                //! max index
                size_t span() const throw();

                //! check if parameter index is handled
                bool handles(const size_t iparam) const throw();

                //! append a primary variable
                template <typename ID>
                variables & operator<<(const ID &id) {
                    const handle h = new primary(id,size()+1);
                    grow(h);
                    return *this;
                }

                //! query variable
                template <typename ID> inline
                const variable & operator[](const ID &id) const
                {
                    return fetch(id);
                }

                //! append a replica variable
                template <typename TARGET>
                variables & operator()(const TARGET &target, const variable &var)
                {
                    const handle src = &var;
                    const handle tgt = new replica(target,src);
                    grow(tgt);
                    return *this;
                }

                //! append replica with same name
                variables & operator<<(const variable &var);


                //! access by name R/O
                template <typename T, typename ID>
                const T & operator()(const readable<T> &arr, const ID &id) const
                {
                    return fetch(id)(arr);
                }

                //! access by name R/W
                template <typename T, typename ID>
                T & operator()(writable <T> &arr, const ID &id) const
                {
                    return fetch(id)(arr);
                }

                //! display name array
                template <typename T> inline
                std::ostream & display(std::ostream &os, const readable<T> &arr, const char *pfx=NULL) const
                {
                    const size_t w = width();
                    if(!pfx) pfx="";
                    for(const_iterator it=begin();it!=end();++it)
                    {
                        const variable &v = **it;
                        os << pfx << ios::align(v.name,w) << " = " << v(arr) << std::endl;
                    }
                    return os;
                }

                //! load all values in span to the same value
                template <typename T> inline
                void ld(writable<T> &arr, typename writable<T>::param_type value )
                {
                    for(size_t i=span();i>0;--i) arr[i] = value;
                }


                //! load column-separated to the same value
                template <typename T, typename ID> inline
                void ld(writable<T> &arr, const ID &descr, typename writable<T>::param_type value)
                {
                    tokenizer tkn(descr);
                    while(tkn.find(is_sep))
                    {
                        const string id(tkn.token(),tkn.units());
                        var(arr,id) = value;
                    }
                }


                //! turn on variables list
                template <typename ID> inline
                void on(writable<bool> &used, const ID &descr)
                {
                    ld<bool,ID>(used,descr,true);
                }

                //! turn off variables list
                template <typename ID> inline
                void off(writable<bool> &used, const ID &descr)
                {
                    ld<bool,ID>(used,descr,false);
                }

                //! turn on ONLY variables list
                template <typename ID> inline
                void only_on(writable<bool> &used, const ID &descr)
                {
                    ld<bool>(used,false);
                    on(used,descr);
                }

                //! turn off ONLY variables list
                template <typename ID> inline
                void only_off(writable<bool> &used, const ID &descr)
                {
                    ld<bool>(used,true);
                    off(used,descr);
                }


                //! display extensive result
                template <typename OSTREAM,typename T> inline
                OSTREAM & display(OSTREAM &os, const readable<T> &arr, const readable<T> &err, const char *pfx=NULL) const
                {
                    const string prolog = pfx;
                    strings      output;


                    for(const_iterator it=begin();it!=end();++it)
                    {
                        const variable &v = **it;
                        output.push_back(prolog);
                        string &tmp = output.back();
                        tmp += v.name;
                    }
                    align_all(output);

                    {
                        size_t j=1;
                        for(const_iterator it=begin();it!=end();++it,++j)
                        {
                            const variable &v = **it;
                            output[j] += vformat(" = %.15g",double(v(arr)));
                        }
                    }
                    align_all(output);


                    {
                        size_t j=1;
                        for(const_iterator it=begin();it!=end();++it,++j)
                        {
                            const variable &v = **it;
                            output[j] += vformat(" +/- %.15g",double(v(err)));
                        }
                    }
                    align_all(output);

                    {
                        size_t j=1;
                        for(const_iterator it=begin();it!=end();++it,++j)
                        {
                            const variable &v = **it;
                            const T         xx = v(arr);
                            const T         dx = v(err);
                            output[j] += vformat(" | (%6.2f%%)", percent::of<T,2>(xx,dx) );
                        }
                    }
                    align_all(output);

                    for(size_t i=1;i<=output.size();++i)
                    {
                        os << output[i] << '\n';
                    }


                    return os;
                }




            private:
                void grow(const handle &);
                const variable & fetch(const string &id) const;
                const variable & fetch(const char   *id) const;
                static void align_all(strings &out);
                static bool is_sep(const int c) throw();
            };



        }

    }
}

#endif

