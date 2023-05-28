

#include "yack/chem/weasel/linker.hpp"
#include "yack/chem/library.hpp"
#include "yack/apex/natural.hpp"
#include "yack/chem/eqs/lua.hpp"

namespace yack
{
    namespace Chemical
    {
        namespace Weasel
        {
            Linker:: ~Linker() noexcept
            {
            }

#define WEASEL_CONNECT(UUID) YACK_JIVE_CONNECT(UUID,Linker)
#define WEASEL_CONTROL(UUID) YACK_JIVE_CONTROL(UUID,Linker)

            Linker:: Linker() :
            jive::syntax::transcriber("Weasel"),
            names(),
            signs(),
            specs(),
            coefs(),
            troup(),
            sides(),
            reac(),
            prod(),
            codes(),
            alias()
            {
                WEASEL_CONNECT(id);
                WEASEL_CONNECT(plus);
                WEASEL_CONNECT(minus);
                WEASEL_CONNECT(cf);
                WEASEL_CONNECT(nil);
                WEASEL_CONNECT(str);
                WEASEL_CONNECT(rx);

                WEASEL_CONTROL(sp);
                WEASEL_CONTROL(fa);
                WEASEL_CONTROL(xa);
                WEASEL_CONTROL(ac);
                WEASEL_CONTROL(cm);
                WEASEL_CONTROL(eq);
                WEASEL_CONTROL(weasel);
            }


            void Linker:: operator()(const XNode &tree, Library &lib, LuaEquilibria &eqs)
            {
                Target data = { lib, eqs};
                walk(tree,&data);
            }


            void Linker:: clr() noexcept
            {
                names.clear();
                signs.clear();
                specs.clear();
                coefs.clear();
                troup.release();
                sides.clear();
                reac.release();
                prod.release();
                codes.clear();
            }

            Linker::Target & Linker:: tgt()
            {
                assert(NULL!=data);
                return *static_cast<Target *>(data);
            }


            void Linker:: on_init()
            {
                clr();
                alias.clear();
            }

            void Linker:: on_quit() noexcept
            {
                clr();
            }

            void Linker:: on_id(const lexeme &lxm)
            {
                names << lxm.data.to_string();
            }

            void Linker:: on_plus(const lexeme &)
            {
                signs << Plus;
            }

            void Linker:: on_minus(const lexeme &)
            {
                signs << Minus;
            }

            void Linker:: on_cf(const lexeme &lxm)
            {
                static const char here[] = "cf";
                const apn cf = lxm.data.to_apn(here);
                const int nu = cf.cast_to<unsigned >(here);
                coefs << nu;
            }

            void Linker:: on_sp(const string &, const size_t narg)
            {
                assert(narg>0);
                assert(names.size>0);
                int           z     = 0;
                string        name  = names.pull_tail();
                const size_t  nz    = narg-1;
                if(nz>0) {
                    assert(signs.size>=nz);
                    char zc = 0;
                    int  za = 0;
                    switch( **signs.tail )
                    {
                        case Plus: zc = '+'; za = 1;
                            break;

                        case Minus: zc = '-'; za = -1;
                            break;
                    }
                    for(size_t i=nz;i>0;--i)
                    {
                        signs.cut_tail();
                        name += zc;
                        z    += za;
                    }
                }
                const Species &sp = tgt().lib(name,z);
                specs << sp;
            }

            void Linker:: on_weasel(const string &, const size_t) { }


            void Linker:: on_fa(const string &, const size_t narg)
            {
                actor(narg);
            }

            void Linker:: on_xa(const string &, const size_t narg)
            {
                actor(narg);
            }

            void Linker:: actor(const size_t narg)
            {
                assert(1==narg||2==narg);
                assert(specs.size>0);
                const Species &sp = *specs.pull_tail();
                unsigned       nu = 1;
                if(2==narg) {
                    assert(coefs.size>0);
                    nu = coefs.pull_tail();
                }
                troup.push_back( new Actor(nu,sp) );
                std::cerr << "troup = " << troup << std::endl;
            }

            void Linker:: on_ac(const string &, const size_t narg)
            {
                assert(troup.size>=narg);
                {
                    Actors none;
                    sides << none;
                }
                Actors &ac = **sides.tail;
                for(size_t i=narg;i>0;--i)
                {
                    ac.push_front( troup.pop_back() );
                }
                std::cerr << "ac=" << ac << std::endl;
            }

            void Linker:: on_nil(const lexeme &)
            {
                Actors none;
                sides << none;
            }

            void Linker:: on_cm(const string &, const size_t narg)
            {
                assert(1==narg||2==narg);
                assert(sides.size>=narg);
                prod.swap_with(**sides.tail); sides.cut_tail();
                if(narg>1)
                {
                    reac.swap_with(**sides.tail); sides.cut_tail();
                }
                else
                {
                    reac.release();
                }
                std::cerr << "reac=" << reac << std::endl;
                std::cerr << "prod=" << prod << std::endl;
            }

            void Linker:: on_str(const lexeme &lxm)
            {
                codes << lxm.data.to_string(1,1);
            }


            void Linker:: on_rx(const lexeme &lxm)
            {
                alias << lxm.data.to_string(1,1);
            }

            void Linker:: on_eq(const string &, const size_t)
            {
                assert(codes.size>0);
                assert(names.size>0);

                // get parameters for Lua
                const string kstr = codes.pull_tail();
                const string name = names.pull_tail();
                Equilibrium &eq   = tgt().eqs(name,kstr);

                // load reactants
                for(const Actor *r=reac.head;r;r=r->next)
                {
                    eq(-static_cast<int>(r->nu),**r);
                }
                reac.release();

                // load products
                for(const Actor *p=prod.head;p;p=p->next)
                {
                    eq(p->nu,**p);
                }
                prod.release();

                
            }


        }

    }

}


