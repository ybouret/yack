
#include "yack/jive/syntax/translator.hpp"
#include "yack/type/temporary.hpp"

namespace yack
{
    namespace jive
    {
        namespace syntax
        {
            translator:: translator() throw() : depth(0), data(NULL)
            {
            }

            translator:: ~translator() throw()
            {
            }


            std::ostream & translator:: indent(std::ostream &os) const
            {
                os << "|-";
                for(unsigned i=depth;i>0;--i) os << '-';
                return os;
            }

            void translator:: on_init() {}

            void translator:: on_quit() throw()
            {
            }



            void translator:: walk(const xnode &tree, void *user_data)
            {
                const temporary<void *> tmp(data,user_data);
                depth = 0;
                try {
                    on_init();
                    inspect(&tree);
                    on_quit();
                }
                catch(...)
                {
                    on_quit();
                    throw;
                }
            }

            void translator:: on_terminal(const lexeme &lex)
            {
                indent(std::cerr) << "[push] " << lex.name;
                if(lex.data.size) std::cerr << "@'" <<  lex.data << "'";
                std::cerr << std::endl;
            }


            void translator:: on_internal(const string &name, const size_t size)
            {
                indent(std::cerr) << "[call] " << name << "/" << size << std::endl;
            }

            void translator:: inspect(const xnode *node)
            {
                assert(node);
                const rule &r = **node;
                switch(r.type)
                {
                    case terminal_type:
                        on_terminal(node->lex());
                     break;

                    case internal_type:
                        ++depth; {
                            const xlist     &ch = node->sub();
                            for(const xnode *xn = ch.head;xn;xn=xn->next)
                                inspect(xn);
                            on_internal(*r.name,ch.size);
                        }
                        --depth;
                        break;
                }
            }

        }
    }
}

