
//! \file

#ifndef YACK_AQUEOUSE_WEASEL_LINKER_INCLUDED
#define YACK_AQUEOUSE_WEASEL_LINKER_INCLUDED 1

#include "yack/jive/syntax/translator.hpp"
#include "yack/aqueous/weasel/types.hpp"
#include "yack/sequence/list.hpp"

namespace yack
{
    namespace aqueous
    {
        namespace weasel
        {
            class linker : public jive::syntax::translator
            {
            public:
                explicit linker();
                virtual ~linker() noexcept;
                

                list<sign_type> signs;

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(linker);
                void         cleanup() noexcept;
                virtual void on_init();
                virtual void on_quit() noexcept;
            };
        }

    }

}

#endif
