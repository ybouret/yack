
//! \file

#ifndef YACK_AQUEOUSE_WEASEL_LINKER_INCLUDED
#define YACK_AQUEOUSE_WEASEL_LINKER_INCLUDED 1

#include "yack/jive/syntax/translator.hpp"
#include "yack/aqueous/weasel/types.hpp"
#include "yack/sequence/list.hpp"
#include "yack/hashing/perfect.hpp"
#include "yack/apex/natural.hpp"
#include "yack/aqueous/library.hpp"
#include "yack/data/dinky/solo-list.hpp"
#include "yack/data/dinky/solo-repo.hpp"

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

                enum charge {
                    zneg = -1,
                    zpos = +1
                };

                struct params
                {
                    library &lib;
                };

                explicit linker();
                virtual ~linker() noexcept;

                static void simplify(xnode *node); //!< suppress the extra actor leading '+'
                
                const hashing::perfect   terms;
                const hashing::perfect   instr;
                solo_list<charge>        signs;
                solo_list<string>        roots; //!< stack of roots for species name
                solo_list<string>        codes; //!< stack of lua codes for eqs
                solo_list<apn>           coefs; //!< stack of coefficients
                size_t                   voids; //!< number of voids
                solo_repo<const species> specs; //!< compiled species

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(linker);
                void         cleanup() noexcept;
                virtual void on_init();
                virtual void on_quit() noexcept;
                virtual void on_terminal(const lexeme &);
                virtual void on_internal(const string &, const size_t );

                void on_species(const size_t args, library &lib);
                void on_actor(const size_t args);
            };
        }

    }

}

#endif
