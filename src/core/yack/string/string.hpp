//! \file

#ifndef YACK_KERNEL_STRING_INCLUDED
#define YACK_KERNEL_STRING_INCLUDED 1

#include "yack/string/string_.hpp"
#include "yack/container/writable.hpp"

namespace yack
{

    namespace kernel
    {

        //______________________________________________________________________
        //
        //
        //! generic string
        //
        //______________________________________________________________________
        template <typename T>
        class string : public string_, public writable<T>
        {
        public:
            using typename writable<T>::const_type;
            using typename writable<T>::type;

            explicit string();
            virtual ~string() throw();
            string(const string &);    //!< copy
            string & operator=(const string &); //!< copy/swap
            string(const T);
            

            //__________________________________________________________________
            //
            // memory::ro_buffer interface
            //__________________________________________________________________
            virtual size_t measure() const throw(); //!< chars * sizeof(T)

            //__________________________________________________________________
            //
            // readable interface
            //__________________________________________________________________
            virtual size_t      size()                        const throw(); //!< chars
            virtual const_type &operator[](const size_t indx) const throw(); //!< [1..chars+1]

            //__________________________________________________________________
            //
            // writable interface
            //__________________________________________________________________
            virtual type       &operator[](const size_t indx)       throw(); //!< [1..chars]

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void xch(string &) throw(); //!< no-throw exchange
            
        private:
            type *item;
        };

    }

}

#endif

