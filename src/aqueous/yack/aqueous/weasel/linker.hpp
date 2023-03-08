
//! \file

#ifndef YACK_AQUEOUSE_WEASEL_LINKER_INCLUDED
#define YACK_AQUEOUSE_WEASEL_LINKER_INCLUDED 1

#include "yack/jive/syntax/translator.hpp"
#include "yack/aqueous/weasel/types.hpp"
#include "yack/sequence/list.hpp"
#include "yack/hashing/perfect.hpp"
#include "yack/apex/natural.hpp"

namespace yack
{
    namespace aqueous
    {
        namespace weasel
        {
            class linker : public jive::syntax::translator
            {
            public:
                static const char * const clid;
                
                explicit linker();
                virtual ~linker() noexcept;

                static void simplify(xnode *node); //!< suppress the extra actor leading '+'
                
                const hashing::perfect terms;
                const hashing::perfect instr;
                list<sign_type>        signs; //!< stack of signs for species name
                list<string>           roots; //!< stack of roots for species name
                list<string>           codes; //!< stack of lua codes for eqs
                list<apn>              coefs; //!< stack of coefficients
                size_t                 voids; //!< number of voids

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(linker);
                void         cleanup() noexcept;
                virtual void on_init();
                virtual void on_quit() noexcept;
                virtual void on_terminal(const lexeme &);
                virtual void on_internal(const string &, const size_t );
            };
        }

    }

}

#endif
