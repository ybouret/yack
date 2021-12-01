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
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef  typename writable<T>::const_type const_type; //!< alias
            typedef  typename writable<T>::type       type;       //!< alias
            static   const char                       clid[];     //!< class id,
            static size_t  length_of(const T *msg) throw();       //!< strlen alias
            
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit string();                           //!< default empty
            virtual ~string() throw();                   //!< cleanup
            string(const string &);                      //!< copy
            string & operator=(const string &);          //!< copy/swap
            string(const      T);                        //!< single CHAR
            string(const char *);                        //!< copy/convert
            string & operator=(const T) throw();         //!< assign single CHAR
            string & operator=(const char *);            //!< assign/convert

            //! build with capacity, filled or not
            string(const size_t, const as_capacity_t &,bool filled=false); //!< with capacity, empty or 0

            //! build with sub-string
            string(const T *entry, const size_t count);

            //__________________________________________________________________
            //
            // memory::ro_buffer interface
            //__________________________________________________________________
            virtual const char *class_uid() const throw();
            virtual size_t      serialize(ios::ostream &) const;


            //__________________________________________________________________
            //
            //  specific access
            //__________________________________________________________________
            const T * operator()() const throw(); //!< raw R/O access

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
            // output
            //__________________________________________________________________
            std::ostream & display(std::ostream &) const; //!< output method
            ios::ostream & display(ios::ostream &) const; //!< output method

            //! display to std::ostream
            inline friend std::ostream & operator<<(std::ostream &os, const string &s) { return s.display(os); }

            //! display to ios::ostream
            inline friend ios::ostream & operator<<(ios::ostream &os, const string &s) { return s.display(os); }
            
            
            //__________________________________________________________________
            //
            // skip/trim
            //__________________________________________________________________
            string & skip(const size_t n) throw(); //!< skip n first chars
            string & trim(const size_t n) throw(); //!< trim n last chars

            //__________________________________________________________________
            //
            // comparisons
            //__________________________________________________________________

            //! fast equality operator (the difference comes with writable<T>)
            inline friend bool operator==(const string &lhs, const string &rhs) throw()
            {
                return eq(static_cast<const_type*>(lhs.block),lhs.chars,
                          static_cast<const_type*>(rhs.block),rhs.chars);
            }
           
            //! fast equality string/message
            inline friend bool operator==(const string &lhs, const T *rhs) throw()
            {
                return eq(static_cast<const_type*>(lhs.block),lhs.chars,
                          rhs,length_of(rhs));
            }

            //! fast equality message/string
            inline friend bool operator==(const T *lhs, const string &rhs) throw()
            {
                return eq(lhs,length_of(lhs),
                          static_cast<const_type*>(rhs.block),rhs.chars);
            }
            
            //! fast difference string/message
            inline friend bool operator!=(const string &lhs, const T *rhs) throw()
            {
                return neq(static_cast<const_type*>(lhs.block),lhs.chars,
                          rhs,length_of(rhs));
            }
            
            //! fast difference message/string
            inline friend bool operator!=(const T *lhs, const string &rhs) throw()
            {
                return neq(lhs,length_of(lhs),
                          static_cast<const_type*>(rhs.block),rhs.chars);
            }
            
            
            
            //! to sort strings
            static int compare(const string &lhs, const string &rhs) throw();

            //! self key
            const string &key() const throw();


        private:
            type *item;
            static bool eq(const T *l, const size_t nl,
                           const T *r, const size_t nr) throw();
            
            static bool neq(const T *l, const size_t nl,
                            const T *r, const size_t nr) throw();

            static int cmp(const T *l, const size_t nl,
                           const T *r, const size_t nr) throw();

        };



    }

}

#endif

