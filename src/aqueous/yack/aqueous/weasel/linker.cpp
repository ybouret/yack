
#include "yack/aqueous/weasel/linker.hpp"
#include "yack/system/imported.hpp"
#include "yack/lua++/function.hpp"

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
                "rx"
            };

#define wl_plus  0
#define wl_minus 1
#define wl_root  2
#define wl_code  3
#define wl_coef  4
#define wl_void  5
#define wl_rx    6

            static const char *internals_kw[] =
            {
                "weasel",
                "sp",
                "fa",
                "xa",
                "ac",
                "cm",
                "eq"
            };

#define wl_ok 0
#define wl_sp 1
#define wl_fa 2
#define wl_xa 3
#define wl_ac 4
#define wl_cm 5
#define wl_eq 6

            linker:: linker()  :
            jive::syntax::translator(),
            terms(YACK_HASHING_PERFECT(terminals_kw)),
            instr(YACK_HASHING_PERFECT(internals_kw)),
            signs(),
            roots(),
            codes(),
            coefs(),
            specs(),
            folks(),
            sides(),
            reacs(),
            prods()
            {
            }

            linker:: ~linker() noexcept
            {
            }

            void linker:: cleanup() noexcept
            {
                prods.release();
                reacs.release();
                sides.release();
                folks.release();
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
                cleanup();
                found.clear();
            }

            void linker:: on_quit() noexcept
            {
                cleanup();
            }
            
            void linker:: on_terminal(const lexeme &l)
            {
                //std::cerr << clid; translator::on_terminal(l);

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
                        codes << l.data.to_string(1,1);
                        break;

                    case wl_coef: {
                        const string __ = l.data.to_string();
                        const apn    nu = apn::parse_dec(__(), __.size()); assert(nu>0);
                        coefs << nu;
                    } break;

                    case wl_void: // store a new empty set of actors
                        sides.store( new actors() );
                        break;

                    case wl_rx: // use regular expression
                    {
                        const string rx = l.data.to_string(1,1);;
                        on_rx(rx);
                    } break;

                    default:
                        throw imported::exception(clid,"terminal '%s' not implemented", (*l.name)() );
                }
            }

            void linker:: on_internal(const string &name, const size_t args)
            {
                //std::cerr << clid; translator::on_internal(name,args);

                assert(data);
                params  &usr = *static_cast<params *>(data);
                library &lib = usr.lib;

                switch(instr(name))
                {
                    case wl_sp:  on_species(args,lib); break;
                    case wl_fa:
                    case wl_xa:  on_actor(args);           break;
                    case wl_ac:  on_actors(args);          break;
                    case wl_cm:  on_compound(args);        break;
                    case wl_eq:  assert(3==args); on_eq(); break;
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
                    if(species::verbose)
                        std::cerr << clid << " +" << *(specs.tail) << std::endl;
                }

            }


            unsigned linker:: pull_coeff()
            {
                assert(coefs.size>0);
                const unsigned nu = (*coefs.tail)->cast_to<int>("coefficient");
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
                assert(args<=folks.size);
                actors *A = sides.store( new actors() );
                for(size_t i=args;i>0;--i) A->push_front(folks.pop_back());
            }

            void linker:: on_compound(const size_t args)
            {
                switch(args)
                {
                    case 1: // assume product only
                        assert(sides.size>=1);
                        reacs.store( new actors()  );
                        prods.store( sides.query() );
                        break;

                    case 2: // prepare reac/prod
                        assert(sides.size>=2);
                        prods.store( sides.query() );
                        reacs.store( sides.query() );
                        break;

                    default:
                        throw imported::exception(clid,"invalid compound/%u",unsigned(args));
                }
                assert(reacs.size>0);
                assert(prods.size>0);
            }

            namespace
            {

                class lua_eq : public equilibrium
                {
                public:
                    inline virtual ~lua_eq() noexcept {}
                    inline explicit lua_eq(const string  &uid,
                                           const string  &fid,
                                           const Lua::VM &lvm,
                                           const size_t  idx) :
                    equilibrium(uid,idx),
                    f(lvm,fid)
                    {
                    }

                    inline lua_eq(const lua_eq &other) :
                    equilibrium(other),
                    f(other.f)
                    {
                    }

                    inline virtual equilibrium *clone() const {
                        return new lua_eq(*this);
                    }

                    Lua::Function<double> f;

                private:
                    YACK_DISABLE_ASSIGN(lua_eq);
                    inline virtual double getK(const double t) {
                        return f(t);
                    }
                };
            }

            void linker:: on_eq()
            {

                // sanity check
                assert(roots.size>0);
                assert(reacs.size>0);
                assert(prods.size>0);
                assert(codes.size>0);

                equilibrium    *pEq  = 0;

                // preparing lua/const equilibrium
                {
                    assert(data);
                    params         &usr  = *static_cast<params *>(data);
                    lua_equilibria &eqs  = usr.eqs;
                    Lua::VM        &lvm  = eqs.vm;
                    const string    code = codes.pull_tail();
                    const string    name = roots.pull_tail();
                    const size_t    indx = eqs.next_indx();
                    lvm->getglobal(code);
                    if(LUA_TFUNCTION==lvm->type(-1))
                    {
                        // make a lua eq
                        pEq = & eqs( new lua_eq(name,code,lvm,indx) );
                    }
                    else
                    {
                        // make a constant eq
                        pEq = & eqs( new const_equilibrium(name, lvm->eval<double>(code), indx ) );
                    }
                }

                assert(NULL!=pEq);
                equilibrium &eq = *pEq;
                // filling eq
                auto_ptr<actors> reac = reacs.query();
                auto_ptr<actors> prod = prods.query();
                for(const actor *a=reac->head;a;a=a->next)
                {
                    eq( - int(a->nu), **a );
                }

                for(const actor *a=prod->head;a;a=a->next)
                {
                    eq( int(a->nu), **a );
                }

                if(species::verbose) eq.display(std::cerr << clid  << " +") << std::endl;
            }


           

        }

    }

}

