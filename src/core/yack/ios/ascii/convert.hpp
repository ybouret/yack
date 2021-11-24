

//! \file

#ifndef YACK_ASCII_ENCODED_INCLUDED
#define YACK_ASCII_ENCODED_INCLUDED 1

#include "yack/setup.hpp"
#include "yack/string/fwd.hpp"
#include "yack/type/ints.hpp"
#include "yack/string.hpp"
#include <cstring>

namespace yack
{

    namespace ios
    {

        namespace ascii
        {

            //__________________________________________________________________
            //
            //
            //! convertion from ascii to data
            //
            //__________________________________________________________________
            struct convert
            {

                static int64_t  to_int64(const char   *args, size_t size, const char *ctx = NULL); //!< substring to int64_t
                static int64_t  to_int64(const char   *text, const char *ctx = NULL );             //!< text to int64_t
                static int64_t  to_int64(const string &data, const char *ctx = NULL);              //!< text to int64_t

                static uint64_t to_uint64(const char   *args, size_t size, const char *ctx = NULL); //!< substring to uint64_t
                static uint64_t to_uint64(const char   *text, const char *ctx = NULL);              //!< text to uint64_t
                static uint64_t to_uint64(const string &data, const char *ctx = NULL);              //!< text to uint64_t

                typedef int2type<true>     to_signed;   //!< alias
                typedef int2type<false>    to_unsigned; //!< alias


                //! convert to integer type
                template <typename T> static inline
                T to(const char *args, const size_t size, const char *ctx=NULL)
                {
                    static const int2type< is_signed<T>::value > which = {};
                    return to_integer<T>(args,size,ctx,which);
                }

                //! convert to integer type
                template <typename T> static inline
                T to(const char *text, const char *ctx=NULL)
                {
                    assert(text);
                    return to<T>(text,strlen(text),ctx);
                }

                //! convert to integer type
                template <typename T> static inline
                T to(const string &data, const char *ctx=NULL)
                {
                    return to<T>(data(),data.size(),ctx);
                }




            private:
                template <typename T> static inline
                T to_integer(const char       *args,
                             const size_t     size,
                             const char      *ctx,
                             const to_signed  &)
                {
                    static const int64_t min_value = integral_for<T>::minimum;
                    static const int64_t max_value = integral_for<T>::maximum;
                    const        int64_t value     = to_int64(args,size,ctx);
                    if(value<min_value||value>max_value)
                    {
                        out_of_range_signed(sizeof(T),ctx);
                    }
                    return T(value);
                }


                template <typename T> static inline
                T to_integer(const char       *args,
                             const size_t     size,
                             const char      *ctx,
                             const to_unsigned  &)
                {
                    static const uint64_t max_value = integral_for<T>::maximum;
                    const        uint64_t value     = to_uint64(args,size,ctx);
                    if(value>max_value)
                    {
                        out_of_range_unsigned(sizeof(T),ctx);
                    }
                    return T(value);
                }



                static void out_of_range_signed(const   size_t bytes,const char *ctx);
                static void out_of_range_unsigned(const size_t bytes,const char *ctx);

            };

        }

    }

}
#endif
