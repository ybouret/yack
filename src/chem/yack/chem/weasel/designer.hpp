
//! \file

#ifndef YACK_WEASEL_DESIGNER_INCLUDED
#define YACK_WEASEL_DESIGNER_INCLUDED 1


#include "yack/chem/eqs/lua.hpp"
#include "yack/chem/library.hpp"
#include "yack/jive/module.hpp"

namespace yack
{
    namespace Chemical
    {
        namespace Weasel
        {
            class Linker;
            class Parser;

            //__________________________________________________________________
            //
            //
            //! Designer to populate library/equilibria
            //
            //__________________________________________________________________
            class Designer : public singleton<Designer>
            {
            public:
                //______________________________________________________________
                //
                // definitions
                //______________________________________________________________
                static const char * const       call_sign;       //!< Chemical::Designer
                static const at_exit::longevity life_time = 13;  //!< life time...

                //______________________________________________________________
                //
                //! parse/link the input to library and equilibria
                //______________________________________________________________
                void operator()(jive::module *, Library &lib, LuaEquilibria &eqs);

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(Designer);
                explicit Designer();
                virtual ~Designer() noexcept;
                friend class singleton<Designer>;
                void   cleanup() noexcept;
                void   aliases(Library &lib, LuaEquilibria &eqs);
                Parser                   *parser;
                Linker                   *linker;
            };

        }
    }

}

#endif

