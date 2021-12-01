//! \file

#ifndef YACK_JIVE_TAGS_INCLUDED
#define YACK_JIVE_TAGS_INCLUDED 1

#include "yack/jive/types.hpp"
#include "yack/singleton.hpp"

namespace yack
{
    namespace jive
    {

        //______________________________________________________________________
        //
        //
        //! database of tags
        //
        //______________________________________________________________________
        class tags : public singleton<tags>, public tags_
        {
        public:
            static const at_exit::longevity life_time = 0; //!< singleton life time
            static const char               call_sign[];   //!< singleton call sign

            static string *make(const char *buffer, const size_t buflen); //!< make/create
            static string *make(const char *buffer);                      //!< alias
            static string *make(const char    c);                         //!< alias
            static string *make(const string &s);                         //!< alias
            static string *make(const tag    &t);                         //!< alias
            static string *make(string       *h);                         //!< alias

            static void gv(); //!< save render in jive-tags.[dot|png]

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(tags);
            friend class singleton<tags>;
            virtual ~tags() throw();
            explicit tags() throw();
        };
    }
}

#endif

