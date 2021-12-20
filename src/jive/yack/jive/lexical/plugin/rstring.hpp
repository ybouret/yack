//! \file

#ifndef YACK_JIVE_LEXICAL_RSTRING_INCLUDED
#define YACK_JIVE_LEXICAL_RSTRING_INCLUDED 1

#include "yack/jive/lexical/plugin/string.hpp"

namespace yack
{
    namespace jive
    {

        namespace lexical
        {

            //__________________________________________________________________
            //
            //
            //! single quoted strings
            //
            //__________________________________________________________________
            class rstring : public string_
            {
            public:
                YACK_JIVE_PLUGIN_DECL(rstring); //!< declare

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                virtual ~rstring() throw(); //!< cleanup

                template <typename IDENTIFIER>
                explicit rstring(const IDENTIFIER &uuid,
                                 analyzer         &host) :
                string_(uuid,'\'','\'','"',host)
                {
                }


            private:
                YACK_DISABLE_COPY_AND_ASSIGN(rstring);

            };


        }

    }

}

#endif
