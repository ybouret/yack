//! \file

#ifndef YACK_STRING_OPS_INCLUDED
#define YACK_STRING_OPS_INCLUDED 1


#include "yack/string/string.hpp"

namespace yack
{

    //__________________________________________________________________________
    //
    //
    //! string operations
    //
    //__________________________________________________________________________
    struct strops
    {

        //! trim last bad chars
        template <typename T, typename FUNCTION> static inline
        kernel::string<T> & trim_if(FUNCTION &is_bad, kernel::string<T> &s) noexcept
        {
            size_t n = 0;
            while( (n<s.size()) && is_bad(s[s.size()-n]) ) ++n;
            return s.trim(n);
        }

        //! skip first bad chars
        template <typename T, typename FUNCTION> static inline
        string & skip_if(FUNCTION &is_bad, kernel::string<T> &s) noexcept
        {
            size_t n = 0;
            while(n<s.size() && is_bad(s[n+1]) ) ++n;
            return s.skip(n);
        }

        //! strip
        template <typename T, typename FUNCTION> static inline
        kernel::string<T> & strip_if(FUNCTION &is_bad, kernel::string<T> &s) noexcept
        {
            return skip_if(is_bad,trim_if(is_bad,s));
        }

        //! trim with data
        template <typename T> static inline
        kernel::string<T> & trim_with(const T *bad, const size_t num, kernel::string<T> &s) noexcept
        {
            assert(!(bad==NULL&&num>0));
            const belongs_to<T> is_bad = { bad, num };
            return trim_if(is_bad,s);
        }

        //! skip with data
        template <typename T> static inline
        kernel::string<T> & skip_with(const T *bad, const size_t num, kernel::string<T> &s) noexcept
        {
            assert(!(bad==NULL&&num>0));
            const belongs_to<T> is_bad = { bad, num };
            return skip_if(is_bad,s);
        }

        //! strip with data
        template <typename T> static inline
        kernel::string<T> & strip_with(const T *bad, const size_t num, kernel::string<T> &s) noexcept
        {
            assert(!(bad==NULL&&num>0));
            const belongs_to<T> is_bad = { bad, num };
            return strip_if(is_bad,s);
        }

        static void make_lower(string &) noexcept; //!< make all lower
        static void make_upper(string &) noexcept; //!< make all upper


    private:

        template <typename T>
        struct belongs_to
        {
            const T *entry;
            size_t   count;
            inline bool operator()(const T ch) const noexcept
            {
                const T *p  = entry;
                size_t   n  = count;
                while(n-- > 0) { if (ch == *(p++) ) return true; }
                return false;
            }
        };

    };

}

#endif

