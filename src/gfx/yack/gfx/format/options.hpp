//! \file

#ifndef YACK_GFX_FORMAT_OPTIONS_INCLUDED
#define YACK_GFX_FORMAT_OPTIONS_INCLUDED 1

#include "yack/string.hpp"
#include "yack/associative/suffix/map.hpp"

namespace yack
{
    namespace graphic
    {

        typedef suffix_map<string,string> options_db;

        class options : public options_db
        {
        public:
            explicit options() throw();
            virtual ~options() throw();

            options & operator<<(const string &);


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(options);
            void add(const string &);
        };

    }
}

#endif

