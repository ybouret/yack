
#include "yack/aqueous/weasel/linker.hpp"
#include "yack/system/imported.hpp"

namespace yack
{
    namespace aqueous
    {
        namespace weasel
        {

            const char * const linker::clid = "linker";

            static const char *terms_kw[] =
            {
                "+",
                "-",
                "id",
                "str",
                "cf",
                "."
            };

#define wl_plus  0
#define wl_minus 1
#define wl_root  2
#define wl_code  3
#define wl_coef  4
#define wl_void  5

            linker:: linker()  :
            jive::syntax::translator(),
            terms(YACK_HASHING_PERFECT(terms_kw)),
            signs()
            {
            }

            linker:: ~linker() noexcept
            {
            }

            void linker:: cleanup() noexcept
            {
                voids = 0;
                coefs.free();
                codes.free();
                roots.free();
                signs.free();
            }

            void linker:: simplify(xnode *node)
            {
                assert(node);
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
            
            void linker:: on_terminal(const lexeme &l)
            {
                std::cerr << clid;
                translator::on_terminal(l);
                switch( terms( *l.name ) )
                {
                    case wl_plus:
                        signs << positive;
                        break;

                    case wl_minus:
                        signs << negative;
                        break;

                    case wl_root:
                        roots << l.data.to_string();
                        break;

                    case wl_code:
                        codes << l.data.to_string();
                        break;

                    case wl_coef: {
                        const string __ = l.data.to_string();
                        const apn    nu = apn::parse_dec(__(), __.size());
                        coefs << nu;
                    } break;

                    case wl_void:
                        break;

                    default:
                        throw exception("not implemented for '%s'", (*l.name)() );
                }
            }

            void linker:: on_internal(const string &name, const size_t args)
            {
                std::cerr << clid;
                translator::on_internal(name,args);
            }

        }

    }

}

