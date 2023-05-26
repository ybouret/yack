

#include "yack/chem/weasel/linker.hpp"

namespace yack
{
    namespace Chemical
    {
        namespace Weasel
        {
            Linker:: ~Linker() noexcept
            {
            }

#define WEASEL_CONNECT(UUID) YACK_JIVE_CONNECT(UUID,Linker)
#define WEASEL_CONTROL(UUID) YACK_JIVE_CONTROL(UUID,Linker)

            Linker:: Linker() : jive::syntax::transcriber("Weasel")
            {
                WEASEL_CONNECT(id);
                WEASEL_CONNECT(plus);
                WEASEL_CONNECT(minus);

                WEASEL_CONTROL(sp);
                WEASEL_CONTROL(weasel);
            }

            void Linker:: clr() noexcept
            {
                names.clear();
                signs.clear();
            }

            void Linker:: on_init()
            {
                std::cerr << label << " :: on_init" << std::endl;
                clr();
            }

            void Linker:: on_quit() noexcept
            {
                std::cerr << label << " :: on_quit" << std::endl;
                std::cerr << "names : " << names << std::endl;
                std::cerr << "signs : " << signs << std::endl;
            }

            void Linker:: on_id(const lexeme &lxm)
            {
                names << lxm.data.to_string();
            }

            void Linker:: on_plus(const lexeme &)
            {
                signs << Plus;
            }

            void Linker:: on_minus(const lexeme &)
            {
                signs << Minus;
            }

            void Linker:: on_sp(const string &, const size_t narg)
            {
                
            }

            void Linker:: on_weasel(const string &, const size_t)
            {

            }
        }

    }

}


