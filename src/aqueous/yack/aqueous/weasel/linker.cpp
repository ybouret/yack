
#include "yack/aqueous/weasel/linker.hpp"

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

            void linker:: simplify(xnode *node)
            {
                assert(node);
                //std::cerr << "simplify [" << node->name() << "]" << std::endl;
                const rule &r = **node;
                switch(r.type)
                {
                    case jive::syntax::terminal_type:
                        return;

                    case jive::syntax::internal_type:
                        jive::syntax::xlist &l = node->sub();
                        for(xnode *ch=l.head;ch;ch=ch->next)
                        {
                            simplify(ch);
                        }
                        if(node->name()=="xa")
                        {
                            //std::cerr << "need to cut!" << std::endl;
                            assert(l.size>=2);
                            assert(l.head->name() == "+");
                            delete l.pop_front();
                        }
                        return;
                }
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

