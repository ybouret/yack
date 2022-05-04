//! \file

#ifndef YACK_GFX_FORMAT_OPTIONS_INCLUDED
#define YACK_GFX_FORMAT_OPTIONS_INCLUDED 1

#include "yack/string.hpp"
#include "yack/associative/suffix/map.hpp"

namespace yack
{
    namespace graphic
    {

        //______________________________________________________________________
        //
        //! base type for options
        //______________________________________________________________________
        typedef suffix_map<string,string> options_db;

        //______________________________________________________________________
        //
        //
        //! database of options
        //
        //______________________________________________________________________
        class options : public options_db
        {
        public:
            explicit options() throw();
            virtual ~options() throw();

            //! parse 'key=val[:other_key=other_val]
            options & operator<<(const string &);

            //! parse helper
            options & operator<<(const char   *);
            

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(options);
            void add(const string &);
        };

    }
}

#endif

