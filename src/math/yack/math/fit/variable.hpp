//! \file

#ifndef YACK_FIT_VARIABLE_INCLUDED
#define YACK_FIT_VARIABLE_INCLUDED 1

#include "yack/string.hpp"

namespace yack
{
    namespace math
    {

        namespace fit
        {
            class variable : public object, public counted
            {
            public:
                // methods
                virtual       ~variable()          throw(); //!< cleanup
                const string & key()         const throw(); //!< name
                size_t         operator*()   const throw(); //!< get index
                void           check(const size_t n) const; //!< in 1:n

                template <typename T> inline
                const T & operator()(const readable<T> &arr) const
                {
                    check(arr.size());
                    return arr[ **this ];
                }

                template <typename T> inline
                T & operator()(writable<T> &arr) const
                {
                    check(arr.size());
                    return arr[ **this ];
                }

                friend std::ostream & operator<<(std::ostream &, const variable &);


                // members
                const string name; //!< identifier

                
            protected:
                //! setup
                template <typename ID>
                explicit variable(const ID &id) : name(id) {}

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(variable);
                virtual size_t get_index() const throw() = 0;
            };

        }
    }
}

#endif


