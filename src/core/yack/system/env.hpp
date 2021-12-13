//! \file

#ifndef YACK_SYSTEM_ENV_INCLUDED
#define YACK_SYSTEM_ENV_INCLUDED 1

#include "yack/string.hpp"
#include "yack/associative/glossary.hpp"

namespace yack
{

    //__________________________________________________________________________
    //
    //
    //! environment variables API
    //
    //__________________________________________________________________________
    struct environment
    {

        static void        show();                                    //!< quick show
        static void        get(glossary<string,string>&);             //!< free/fill database
        static bool        get(const string &key, string *val);       //!< query key
        static bool        get(const char   *key, string *val);       //!< query key
        static void        set(const string &key, const string &val); //!< store key
        static void        set(const char   *key, const string &val); //!< store key
        static void        set(const char   *key, const char   *val); //!< store key
        static void        set(const string &key, const char   *val); //!< store key
        static void        clr(const string &key);                    //!< remove
        static void        clr(const char   *key);                    //!< remove

    };

}

#endif

