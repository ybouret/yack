
//! \file

#ifndef YACK_WEASEL_LINKER_INCLUDED
#define YACK_WEASEL_LINKER_INCLUDED 1


#include "yack/jive/syntax/transcriber.hpp"
#include "yack/chem/weasel/types.hpp"
#include "yack/data/dinky/solo-repo.hpp"
#include "yack/chem/eqs/actors.hpp"

namespace yack
{
    namespace Chemical
    {

        class Library;
        class Species;
        class LuaEquilibria;

        namespace Weasel
        {
            class Linker : public jive::syntax::transcriber
            {
            public:
                enum Sign
                {
                    Plus  =  1,
                    Minus = -1
                };

                struct Target
                {
                    Library    &lib;
                    LuaEquilibria &eqs;
                };

                explicit Linker();
                virtual ~Linker() noexcept;

                void operator()(const XNode &, Library &, LuaEquilibria &);

                solo_list<string>   names; //!< stack of names
                solo_list<Sign>     signs; //!< stack of signs
                solo_repo<Species>  specs; //!< stack of species
                solo_list<unsigned> coefs; //!< stack of coefficients
                Actors              troup; //!< stack of actors
                solo_list<Actors>   sides; //!< stack of Actors
                Actors              reac;  //!< prepared reactants
                Actors              prod;  //!< prepared products
                solo_list<string>   codes; //!< stack of constant codes
                solo_list<string>   alias; //!< list of <rx>

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(Linker);
                virtual void on_init();
                virtual void on_quit() noexcept;

                Target &tgt();

                void clr() noexcept;
                void on_id(const lexeme &);       //! ID
                void on_plus(const lexeme &);     //! '+'
                void on_minus(const lexeme &);    //! '-'
                void on_cf(const lexeme &);       //! nu
                void on_nil(const lexeme &);      //!< '.' => empty actors
                void on_str(const lexeme &);      //!< rstring => codes
                void on_rx(const lexeme &);       //!< bstring => alias

                void on_sp(const string &, const size_t);    //!< species
                void on_fa(const string &, const size_t);    //!< first actor
                void on_xa(const string &, const size_t);    //!< extra actor
                void actor(const size_t);                    //!< first or extra actor
                void on_ac(const string &, const size_t);    //!< gather actors
                void on_cm(const string &, const size_t);    //!< create components
                void on_eq(const string &, const size_t);    //!< create an equilibrium
                void on_weasel(const string &,const size_t);
            };
        }
    }

}

#endif

