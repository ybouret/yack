

#include "yack/jive/syntax/grammar.hpp"

#include "yack/jive/syntax/rule/terminal.hpp"
#include "yack/jive/syntax/rule/wildcard/repeat.hpp"
#include "yack/jive/syntax/rule/wildcard/option.hpp"

#include "yack/jive/syntax/rule/compound/aggregate.hpp"
#include "yack/jive/syntax/rule/compound/alternate.hpp"


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



            static inline void connect(ios::ostream &fp, const wildcard *r)
            {
                assert(r);
                r->link(fp,&**r);
                r->end(fp);
            }

            static inline void connect(ios::ostream &fp, const compound *comp)
            {
                assert(comp);
                const bool precise = comp->size>1;

                unsigned i=0;
                for(const component *chld=comp->head;chld;chld=chld->next)
                {
                    comp->link(fp,& **chld);
                    if(precise)
                    {
                        fp("[label=\"%u\"]",++i);
                    }
                    comp->end(fp);
                }


            }



            void grammar:: gv_draw(ios::ostream &fp) const
            {
                digraph_init(fp,"G");

                logo(fp) << '[';
                {
                    const string root_id = *lang + ".grammar";
                    add_label(fp,root_id());
                }
                fp << ",shape=rectangle";
                end(fp << ']');


                // save all rules without linking
                for(const rule *r=rules.head;r;r=r->next)
                {
                    // start rule
                    r->logo(fp) << '[';
                    r->add_label(fp,(*(r->name))());
                    const uint32_t uuid = r->uuid;

                    // decorate rule
                    switch(uuid)
                    {
                        case terminal::mark:
                            fp << ",shape=box";
                            break;

                        case option::mark:
                            fp << ",shape=diamond";
                            break;

                        case repeat::mark:
                            fp << ",shape=hexagon";
                            break;

                        case aggregate::mark:
                            fp << ",shape=house";
                            break;

                        case alternate::mark:
                            fp << ",shape=egg";
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

                        case aggregate::mark:
                            connect(fp,r->as<aggregate>());
                            break;

                        case alternate::mark:
                            connect(fp,r->as<alternate>());
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

