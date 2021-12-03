//! \file

#ifndef YACK_JIVE_TAGS_INCLUDED
#define YACK_JIVE_TAGS_INCLUDED 1

#include "yack/string.hpp"
#include "yack/ptr/ark.hpp"
#include "yack/associative/suffix/set.hpp"
#include "yack/associative/suffix/map.hpp"
#include "yack/singleton.hpp"

namespace yack
{
    namespace jive
    {
        class   pattern;
        typedef ark_ptr<string,string> tag;     //!< shared string
        typedef suffix_set<string,tag> tags_;   //!< base class for database

        //______________________________________________________________________
        //
        //
        //! database of tags
        //
        //______________________________________________________________________
        class tags : public singleton<tags>, public tags_
        {
        public:
            typedef pattern * (*pcreator)();
            typedef suffix_map<string,pcreator> pfactory;
            static const at_exit::longevity life_time = 2; //!< singleton life time
            static const char               call_sign[];   //!< singleton call sign


            static string *make(const char *buffer, const size_t buflen); //!< make/create
            static string *make(const char *buffer);                      //!< alias
            static string *make(const char    c);                         //!< alias
            static string *make(const string &s);                         //!< alias
            static string *make(const tag    &t);                         //!< alias
            static string *make(string       *h);                         //!< alias

            static void gv(); //!< save render in jive-tags.[dot|png]

            static pfactory & factory();
            

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(tags);
            friend class singleton<tags>;
            virtual ~tags() throw();
            explicit tags() throw();
            pfactory pdb;

        };
    }
}

#endif

