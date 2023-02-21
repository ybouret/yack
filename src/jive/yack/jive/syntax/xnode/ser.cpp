#include "yack/jive/syntax/xnode.hpp"
#include "yack/jive/syntax/rule/terminal.hpp"
#include "yack/ios/encoder.hpp"


namespace yack
{
    namespace jive
    {
        namespace syntax
        {
            const char * xnode::class_uid() const noexcept { return clid; }

            size_t xnode:: serialize(ios::ostream &fp) const
            {
                const rule &r       = **this;
                size_t      written = r.name->serialize(fp);
                switch(r.type)
                {
                    case terminal_type: {
                        const lexeme &l = lex();
                        const token  &t = l.data;
                        written += ios::encoder::serialize(fp,t.size);
                        for(const character *ch=t.head;ch;ch=ch->next,++written)
                        {
                            fp.write(char(**ch));
                        }
                    } break;

                    case internal_type:
                    {
                        const list_of<xnode> &l = sub();
                        written += ios::encoder::serialize(fp,l.size);
                        for(const xnode *node=l.head;node;node=node->next)
                        {
                            written += node->serialize(fp);
                        }
                    } break;
                }

                return written;
            }
        }

    }

}

