//! \file

#ifndef YACK_KERNEL_STRING_INCLUDED
#define YACK_KERNEL_STRING_INCLUDED 1

#include "yack/string/string_.hpp"
#include "yack/container/writable.hpp"
#include "yack/container/as-capacity.hpp"

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
            typedef  typename writable<T>::const_type const_type;
            typedef  typename writable<T>::type       type;

            
            explicit string();                   //!< default empty
            virtual ~string() throw();           //!< cleanup
            string(const string &);              //!< copy
            string & operator=(const string &);  //!< copy/swap
            string(const      T);                //!< single CHAR
            string(const char *);                //!< copy/convert
            string & operator=(const T) throw();         //!< assign single CHAR
            string & operator=(const char *);            //!< assign/convert
            string(const size_t, const as_capacity_t &); //!< with capacity, empty


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
            void clear()       throw(); //!< clear data


            //__________________________________________________________________
            //
            // catenation
            //__________________________________________________________________
            static string cat(const string &lhs, const string &rhs); //!< cat
            static string cat(const string &lhs, const char   *rhs); //!< cat/convert
            static string cat(const char   *lhs, const string &rhs); //!< cat/convert
            static string cat(const string &lhs, const T       rhs); //!< cat
            static string cat(const T       lhs, const string &rhs); //!< cat

            inline friend string operator+(const string &lhs, const string &rhs) { return cat(lhs,rhs); } //!< alias
            inline friend string operator+(const string &lhs, const char   *rhs) { return cat(lhs,rhs); } //!< alias
            inline friend string operator+(const char   *lhs, const string &rhs) { return cat(lhs,rhs); } //!< alias
            inline friend string operator+(const string &lhs, const T       rhs) { return cat(lhs,rhs); } //!< alias
            inline friend string operator+(const T       lhs, const string &rhs) { return cat(lhs,rhs); } //!< alias

            string & operator+=(const string &); //!< cat
            string & operator+=(const T       ); //!< cat
            string & operator+=(const char   *); //!< cat

            //__________________________________________________________________
            //
            // skip/trim
            //__________________________________________________________________
            string & skip(const size_t n) throw();
            string & trim(const size_t n) throw();


        private:
            type *item;
        };



    }

}

#endif

