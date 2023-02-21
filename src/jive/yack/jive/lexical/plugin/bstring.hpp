//! \file

#ifndef YACK_JIVE_LEXICAL_BSTRING_INCLUDED
#define YACK_JIVE_LEXICAL_BSTRING_INCLUDED 1

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
            //! bracket string
            //
            //__________________________________________________________________
            class bstring : public string_
            {
            public:
                YACK_JIVE_PLUGIN_DECL(bstring); //!< declare

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                virtual ~bstring() noexcept; //!< cleanup

                //! setup with brackets
                template <typename IDENTIFIER>
                explicit bstring(const IDENTIFIER &uuid,
                                 analyzer         &host) :
                string_(uuid,'<','>',0,host)
                {
                }


            private:
                YACK_DISABLE_COPY_AND_ASSIGN(bstring);

            };


        }

    }

}

#endif
