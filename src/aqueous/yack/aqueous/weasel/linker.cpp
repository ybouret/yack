
#include "yack/aqueous/weasel/linker.hpp"
#include "yack/system/imported.hpp"

namespace yack
{
    namespace aqueous
    {
        namespace weasel
        {

            const char * const linker::clid = "linker";

            static const char *terminals_kw[] =
            {
                "+",
                "-",
                "id",
                "str",
                "cf",
                ".",
            };

#define wl_plus  0
#define wl_minus 1
#define wl_root  2
#define wl_code  3
#define wl_coef  4
#define wl_void  5

            static const char *internals_kw[] =
            {
                "weasel",
                "sp",
                "fa",
                "xa",
                "ac"
            };

#define wl_ok 0
#define wl_sp 1
#define wl_fa 2
#define wl_xa 3
#define wl_ac 4

            linker:: linker()  :
            jive::syntax::translator(),
            terms(YACK_HASHING_PERFECT(terminals_kw)),
            instr(YACK_HASHING_PERFECT(internals_kw)),
            signs()
            {
            }

            linker:: ~linker() noexcept
            {
            }

            void linker:: cleanup() noexcept
            {
                sides.release();
                folks.release();
                voids = 0;
                coefs.clear();
                codes.clear();
                roots.clear();
                signs.clear();
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
                        signs << zpos;
                        break;

                    case wl_minus:
                        signs << zneg;
                        break;

                    case wl_root:
                        roots << l.data.to_string();
                        break;

                    case wl_code:
                        codes << l.data.to_string();
                        break;

                    case wl_coef: {
                        const string __ = l.data.to_string();
                        const apn    nu = apn::parse_dec(__(), __.size()); assert(nu>0);
                        coefs << nu;
                    } break;

                    case wl_void:
                        break;

                    default:
                        throw imported::exception(clid,"no terminal '%s'", (*l.name)() );
                }
            }

            void linker:: on_internal(const string &name, const size_t args)
            {
                std::cerr << clid;
                translator::on_internal(name,args);

                assert(data);
                params  &usr = *static_cast<params *>(data);
                library &lib = usr.lib;

                switch(instr(name))
                {
                    case wl_sp:  on_species(args,lib); break;
                    case wl_fa:
                    case wl_xa:  on_actor(args);       break;
                    case wl_ac:  on_actors(args);      break;
                        break;
                    case wl_ok: return;
                    default:
                        throw imported::exception(clid,"no internal'%s'", name() );
                }

            }

            void linker:: on_species(const size_t args, library &lib)
            {
                string       id = roots.pull_tail();
                size_t       n  = args-1;
                int          z  = 0;
                while(n-- > 0)
                {
                    switch( signs.pull_tail() )
                    {
                        case zpos: ++z; id += '+'; break;
                        case zneg: --z; id += '-'; break;
                    }
                }
                const species *sp = lib.query(id);
                if(sp)
                {
                    specs << *sp;
                }
                else
                {
                    specs << lib(id,z);
                }

            }


            unsigned linker:: pull_coeff()
            {
                assert(coefs.size>0);
                const unsigned nu = (*coefs.tail)->cast_to<unsigned>("coefficient");
                coefs.cut_tail();
                return nu;
            }

            void linker:: on_actor(const size_t args)
            {
                assert(specs.size>0);
                switch(args)
                {
                    case 1: { // standalone component
                        folks.push_back( new actor(*specs.pull_tail(),1) );
                    } break;

                    case 2: {
                        folks.push_back( new actor(*specs.pull_tail(),pull_coeff()) );
                    } break;

                    default:
                        throw imported::exception(clid,"invalid actor/%u",unsigned(args));
                }
            }

            void linker:: on_actors(const size_t args)
            {
                //std::cerr << "extracting #" << args << " from " << folks << std::endl;
                assert(args<=folks.size);
                actors *A = sides.store( new actors() );
                for(size_t i=args;i>0;--i) A->push_front(folks.pop_back());
                std::cerr << "\t -> " << *A << std::endl;
            }


        }

    }

}

