//! \file

#ifndef YACK_FIT_VARIABLES_INCLUDED
#define YACK_FIT_VARIABLES_INCLUDED 1

#include "yack/math/fit/replica.hpp"
#include "yack/associative/suffix/set.hpp"
#include "yack/ios/fmt/align.hpp"

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
                inline variables & operator<<(const variable &var)
                {
                    return (*this)(var.name,var);
                }

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
                std::ostream & display(std::ostream &os, const readable<T> &arr) const
                {
                    const size_t w = width();
                    for(const_iterator it=begin();it!=end();++it)
                    {
                        const variable &v = **it;
                        os << ios::align(v.name,w) << " = " << v(arr) << std::endl;
                    }
                    return os;
                }



            private:
                void grow(const handle &);
                const variable & fetch(const string &id) const;
                const variable & fetch(const char   *id) const;
            };



        }

    }
}

#endif

