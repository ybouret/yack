#include "yack/jive/syntax/xnode.hpp"
#include "yack/jive/syntax/rule/terminal.hpp"


namespace yack
{
    namespace jive
    {
        namespace syntax
        {

            void xnode:: gv_(ios::ostream &fp)   const
            {
                const rule &r = **this;

                logo(fp);
                string l = *r.name;
                switch(r.type)
                {
                    case internal_type: {
                        // <internal>
                        fp << '[';
                        add_label(fp,l());
                        fp << ",shape=house";
                        fp << ']';
                        end(fp);
                        const list_of<xnode> &chld = sub();
                        const bool            show = chld.size>1;
                        unsigned              indx = 1;
                        for(const xnode *node=chld.head;node;node=node->next,++indx)
                        {
                            node->gv_(fp);
                            link(fp,node);
                            if(show)
                            {
                                fp("[label=\"%u\"]",indx);
                            }
                            end(fp);
                        }
                        // <internal/>
                    } break;

                    case terminal_type: {
                        // <terminal>
                        const terminal &t = *r.as<terminal>();
                        switch(t.role)
                        {
                            case standard:
                                l += " = '";
                                l += (*lex()).to_string();
                                l += "'";
                                fp << '[';
                                add_label(fp,l());
                                fp << ",shape=box";
                                fp << ']';
                                break;

                            case univocal:
                                fp << '[';
                                add_label(fp,l());
                                fp << ",shape=box,style=rounded";
                                fp << ']';
                                break;

                            case division:
                                fp << '[';
                                add_label(fp,l());
                                fp << ",shape=box,style=dashed";
                                fp << ']';
                                break;
                        }
                        end(fp);
                        // <terminal/>
                    } break;
                }

            }

            void xnode:: gv(const string &filename) const
            {
                {
                    ios::ocstream fp(filename);
                    digraph_init(fp,"G");
                    gv_(fp);
                    digraph_quit(fp);
                }

                ios::vizible::render(filename);
            }

            void xnode:: gv(const char *filename) const
            {
                const string _(filename);
                gv(_);
            }



        }

    }

}


