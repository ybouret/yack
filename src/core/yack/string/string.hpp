//! \file

#ifndef YACK_KERNEL_STRING_INCLUDED
#define YACK_KERNEL_STRING_INCLUDED 1

#include "yack/string/string_.hpp"
#include "yack/container/writable.hpp"

namespace yack
{

    namespace kernel
    {

        
        template <typename T>
        class string : public string_, public writable<T>
        {
        public:
            using typename writable<T>::const_type;
            using typename writable<T>::type;

            explicit string();
            virtual ~string() throw();

            virtual size_t measure() const throw();
            virtual size_t size() const throw();
            virtual const_type &operator[](const size_t indx) const throw(); //!< [1..chars+1]
            virtual type       &operator[](const size_t indx)       throw(); //!< [1..chars]

        private:
            type *item;
        };

    }

}

#endif

