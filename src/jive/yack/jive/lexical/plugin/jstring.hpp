
//! \file

#ifndef YACK_JIVE_LEXICAL_JSTRING_INCLUDED
#define YACK_JIVE_LEXICAL_JSTRING_INCLUDED 1

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
            //! double quoted strings
            //
            //__________________________________________________________________
            class jstring : public string_
            {
            public:
                YACK_JIVE_PLUGIN_DECL(jstring); //!< declare

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                virtual ~jstring() throw(); //!< cleanup

                template <typename IDENTIFIER>
                explicit jstring(const IDENTIFIER &uuid,
                                 analyzer         &host) :
                string_(uuid,'\"','\"','\'',host)
                {
                }
                

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(jstring);

            };


        }

    }

}

#endif
