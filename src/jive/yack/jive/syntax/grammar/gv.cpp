

#include "yack/jive/syntax/grammar.hpp"

#include "yack/jive/syntax/rule/terminal.hpp"
#include "yack/jive/syntax/rule/wildcard/repeat.hpp"
#include "yack/jive/syntax/rule/wildcard/option.hpp"

#include "yack/exception.hpp"
#include "yack/string/cxx-name.hpp"

namespace yack
{
    namespace jive
    {
        namespace syntax
        {

            void grammar:: gv() const
            {
                const string fn = cxx_name::of(*lang) + ".dot";
                {
                    ios::ocstream fp(fn);
                    gv_draw(fp);
                }
                render(fn);
            }

            static inline void decorate(ios::ostream &fp, const terminal *r)
            {
                r->add_label(fp,(*(r->name))());
            }

            static inline void connect(ios::ostream &fp, const wildcard *r)
            {
                r->link(fp,&**r);

                r->end(fp);
            }



            void grammar:: gv_draw(ios::ostream &fp) const
            {
                digraph_init(fp,"G");

                logo(fp) << '[';
                add_label(fp,(*lang)());
                fp << ",shape=rectangle";
                end(fp << ']');


                // save all rules without linking
                for(const rule *r=rules.head;r;r=r->next)
                {
                    // start rule
                    r->logo(fp) << '[';
                    const uint32_t uuid = r->uuid;

                    // decorate rule
                    switch(uuid)
                    {
                        case terminal::mark:
                            decorate(fp,r->as<terminal>());
                            break;

                        case option::mark:
                            r->add_label(fp,(*(r->name))());
                            fp << ",shape=diamond";
                            break;

                        case repeat::mark:
                            r->add_label(fp,(*(r->name))());
                            fp << ",shape=hexagon";
                            break;

                        default:
                            throw exception("[%s] unhandled gv %s",(*lang)(), yack_fourcc(uuid));
                    }

                    // finish rule
                    end(fp<< ']');
                }

                // link
                for(const rule *r=rules.head;r;r=r->next)
                {
                    const uint32_t uuid = r->uuid;

                    // decorate rule
                    switch(uuid)
                    {
                        case terminal::mark:
                            break;

                        case option::mark:
                            connect(fp,r->as<option>());
                            break;

                        case repeat::mark:
                            connect(fp,r->as<repeat>());
                            break;

                        default:
                            throw exception("[%s] unhandled gv %s",(*lang)(), yack_fourcc(uuid));
                    }

                }


                // root->top
                {
                    const rule *r = rules.head;
                    if(r)
                    {
                        end(link(fp,r));
                    }
                }

                digraph_quit(fp);
            }

        }

    }
}

