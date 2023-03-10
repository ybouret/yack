
//! \file

#ifndef YACK_AQUEOUSE_WEASEL_LINKER_INCLUDED
#define YACK_AQUEOUSE_WEASEL_LINKER_INCLUDED 1

#include "yack/jive/syntax/translator.hpp"
#include "yack/aqueous/weasel/types.hpp"
#include "yack/sequence/list.hpp"
#include "yack/hashing/perfect.hpp"
#include "yack/apex/natural.hpp"
#include "yack/aqueous/library.hpp"
#include "yack/aqueous/lua/equilibria.hpp"
#include "yack/data/dinky/solo-list.hpp"
#include "yack/data/dinky/solo-repo.hpp"

namespace yack
{
    namespace aqueous
    {
        
        namespace weasel
        {
            //__________________________________________________________________
            //
            //
            //! convert syntax tree intro species+equilibria
            //
            //__________________________________________________________________
            class linker : public jive::syntax::translator
            {
            public:
                //______________________________________________________________
                //
                // definitions
                //______________________________________________________________
                static const char * const clid; //!< linker

                //! algebraic unit charge
                enum charge {
                    zneg = -1, //!< negative
                    zpos = +1  //!< positive
                };

                //! lightweight parameters
                struct params
                {
                    library        &lib; //!< holding library
                    lua_equilibria &eqs; //!< equilibria with a Lua::VM
                };

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                explicit linker();
                virtual ~linker() noexcept;

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________

                //! suppress the extra actor leading '+', mandatory
                static void simplify(xnode *node);

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const hashing::perfect   terms; //!< terminals perfect hashing
                const hashing::perfect   instr; //!< internals perfect hashing
                solo_list<charge>        signs; //!< stack of signs
                solo_list<string>        roots; //!< stack of roots for species name
                solo_list<string>        codes; //!< stack of lua codes for eqs
                solo_list<apn>           coefs; //!< stack of coefficients
                solo_repo<const species> specs; //!< compiled species
                actors                   folks; //!< compiled actors
                cxx_pool_of<actors>      sides; //!< compiled sides
                cxx_pool_of<actors>      reacs; //!< store reacs for eq
                cxx_pool_of<actors>      prods; //!< store prods for eq

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(linker);
                void         cleanup() noexcept;
                virtual void on_init();
                virtual void on_quit() noexcept;
                virtual void on_terminal(const lexeme &);
                virtual void on_internal(const string &, const size_t );

                void     on_species(const size_t args, library &lib);
                void     on_actor(const size_t args);
                unsigned pull_coeff();
                void     on_actors(const size_t args);
                void     on_compound(const size_t args);
                void     on_eq();
                void     on_rx(const string &rx);
            };
        }

    }

}

#endif
