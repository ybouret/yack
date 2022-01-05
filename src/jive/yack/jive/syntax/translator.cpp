
#include "yack/jive/syntax/translator.hpp"

namespace yack
{
    namespace jive
    {
        namespace syntax
        {
            translator:: translator() throw() : depth(0)
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



            void translator:: walk(const xnode &tree)
            {
                depth = 0;
                inspect(&tree);
            }

            void translator:: on_terminal(const string &name, const token &data)
            {
                indent(std::cerr) << "[push] " << name;
                if(data.size) std::cerr << "@'" << data << "'";
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
                    case terminal_type: {
                        const lexeme &lex = node->lex();
                        on_terminal(*lex.name,*lex);
                    } break;

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

