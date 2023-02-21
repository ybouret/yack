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
            static size_t  length_of(const T *msg) noexcept;       //!< strlen alias
            
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit string();                           //!< default empty
            virtual ~string() noexcept;                   //!< cleanup
            string(const string &);                      //!< copy
            string & operator=(const string &);          //!< copy/swap
            string(const      T);                        //!< single CHAR
            string(const char *);                        //!< copy/convert
            string & operator=(const T) noexcept;         //!< assign single CHAR
            string & operator=(const char *);            //!< assign/convert

            //! build with capacity, filled or not
            string(const size_t, const as_capacity_t &,bool filled=false); //!< with capacity, empty or 0

            //! build with sub-string
            string(const T *entry, const size_t count);

            //__________________________________________________________________
            //
            // memory::ro_buffer interface
            //__________________________________________________________________
            virtual const char *class_uid() const noexcept;
            virtual size_t      serialize(ios::ostream &) const;


            //__________________________________________________________________
            //
            //  specific access
            //__________________________________________________________________
            const T * operator()() const noexcept; //!< raw R/O access

            //__________________________________________________________________
            //
            // memory::ro_buffer interface
            //__________________________________________________________________
            virtual size_t measure() const noexcept; //!< chars * sizeof(T)

            //__________________________________________________________________
            //
            // readable interface
            //__________________________________________________________________
            virtual size_t      size()                        const noexcept; //!< chars
            virtual const_type &operator[](const size_t indx) const noexcept; //!< [1..chars+1]

            //__________________________________________________________________
            //
            // writable interface
            //__________________________________________________________________
            virtual type       &operator[](const size_t indx)       noexcept; //!< [1..chars]
            
            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void        xch(string &) noexcept; //!< no-throw exchange
            void        clear()       noexcept; //!< clear data
            type       &front()       noexcept; //!< (*this)[1]
            const_type &front() const noexcept; //!< (*this)[1]
            type       &back()        noexcept; //!< (*this)[chars]
            const_type &back()  const noexcept; //!< (*this)[chars]

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

            //! display to std::ostream
            inline friend std::ostream & operator<<(std::ostream &os, const string &s) { return s.display(os); }

            
            
            //__________________________________________________________________
            //
            // skip/trim
            //__________________________________________________________________
            string & skip(const size_t n) noexcept; //!< skip n first chars
            string & trim(const size_t n) noexcept; //!< trim n last chars

            //__________________________________________________________________
            //
            // comparisons
            //__________________________________________________________________

            //! fast equality operator (the difference comes with writable<T>)
            inline friend bool operator==(const string &lhs, const string &rhs) noexcept
            {
                return eq(static_cast<const_type*>(lhs.block),lhs.chars,
                          static_cast<const_type*>(rhs.block),rhs.chars);
            }
           
            //! fast equality string/message
            inline friend bool operator==(const string &lhs, const T *rhs) noexcept
            {
                return eq(static_cast<const_type*>(lhs.block),lhs.chars,
                          rhs,length_of(rhs));
            }

            //! fast equality message/string
            inline friend bool operator==(const T *lhs, const string &rhs) noexcept
            {
                return eq(lhs,length_of(lhs),
                          static_cast<const_type*>(rhs.block),rhs.chars);
            }
            
            //! fast difference string/message
            inline friend bool operator!=(const string &lhs, const T *rhs) noexcept
            {
                return neq(static_cast<const_type*>(lhs.block),lhs.chars,
                          rhs,length_of(rhs));
            }
            
            //! fast difference message/string
            inline friend bool operator!=(const T *lhs, const string &rhs) noexcept
            {
                return neq(lhs,length_of(lhs),
                          static_cast<const_type*>(rhs.block),rhs.chars);
            }
            
            
            
            //! to compare strings
            static int compare(const string &lhs, const string &rhs) noexcept;

            static int callcmp(const string &lhs, const string &rhs) noexcept; //!< call cmp
            static int callcmp(const string &lhs, const T      *rhs) noexcept; //!< call cmp
            static int callcmp(const T      *lhs, const string &rhs) noexcept; //!< call cmp

            //! helper for comparisons
#define YACK_STRING_CMP(OP) \
inline bool friend operator OP (const string &lhs, const string &rhs) noexcept { return compare(lhs,rhs) OP 0; }\
inline bool friend operator OP (const string &lhs, const char   *rhs) noexcept { return compare(lhs,rhs) OP 0; }\
inline bool friend operator OP (const char   *lhs, const string &rhs) noexcept { return compare(lhs,rhs) OP 0; }

            //! declare all comparison
#define YACK_STRING_CMPS() YACK_STRING_CMP(<) YACK_STRING_CMP(>) YACK_STRING_CMP(<=) YACK_STRING_CMP(>=)

            YACK_STRING_CMPS()

            //! self key
            const string &key() const noexcept;


        private:
            type *item;
            static bool eq(const T *l, const size_t nl,
                           const T *r, const size_t nr) noexcept;
            
            static bool neq(const T *l, const size_t nl,
                            const T *r, const size_t nr) noexcept;

            static int cmp(const T *l, const size_t nl,
                           const T *r, const size_t nr) noexcept;

        };



    }

}

#endif

