
#include "yack/aqueous/weasel/linker.hpp"
#include "yack/aqueous/weasel/types.hpp"

namespace yack
{
    namespace aqueous
    {
        namespace weasel
        {

            linker:: linker()  : jive::syntax::translator()
            {
            }

            linker:: ~linker() noexcept
            {
            }

            void linker:: cleanup() noexcept
            {
                signs.free();
            }

            

            void linker:: on_init()
            {
                std::cerr << "linker+init" << std::endl;
                cleanup();
            }

            void linker:: on_quit() noexcept
            {
                std::cerr << "linker+quit" << std::endl;
                cleanup();
            }
            
            
        }

    }

}

