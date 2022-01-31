

#include "yack/chem/lua/equilibria.hpp"
#include "yack/chem/library.hpp"
#include "yack/exception.hpp"
#include "yack/string/tokenizer.hpp"
#include "yack/string/ops.hpp"
#include "yack/lua++/function.hpp"
#include "yack/ptr/auto.hpp"

namespace yack
{

    namespace chemical
    {

        lua_equilibria:: ~lua_equilibria() throw()
        {

        }

        lua_equilibria:: lua_equilibria() : Lua::VM( new Lua::State() )
        {
            
        }

        void lua_equilibria:: operator()(const char   *expr, library &lib)
        {
            const string _(expr);
            return (*this)(_,lib);
        }

        namespace
        {
            class lua_eq : public equilibrium
            {
            public:
                explicit lua_eq(const string  &eid,
                                const Lua::VM &lvm,
                                const string   kfn) :
                equilibrium(eid), k(lvm,kfn)
                {
                }

                virtual ~lua_eq() throw()
                {

                }

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(lua_eq);
                mutable Lua::Function<double> k;
                inline virtual double getK(const double t) const
                {
                    return k(t);
                }
            };
        }


#define LBRACK '['

        void lua_equilibria:: operator()(const string &expr, library &lib)
        {

            static const char fn[] = "chemical::equilibria";

            vector<string> words;
            tokenizer::split_with(library::separator,words,expr);
            if(words.size()<=0) return;

            for(size_t i=words.size();i>0;--i)
            {
                strops::clean_with(" \t",2,words[i]);
            }

            string &entry = words[1];
            if(entry.size()<=0) throw exception("%s: invalid first entry '%s' in '%s'",fn, entry(), expr());
            switch(entry[1])
            {
                case LBRACK:
                    // assuming species only
                    for(size_t i=words.size();i>0;--i) (void) lib(words[i]);
                    break;

                case '@': {
                    // assuming full equilibrium
                    entry.skip(1);
                    if(entry.size()<=0) throw exception("%s: invalid empty name in '%s'",fn, expr());
                    if(words.size()!=3) throw exception("%s: invalid format '%s'",fn,expr());
                    string &K = words[3];
                    if(K.size()<=0) throw exception("%s: invalid empty constant in '%s'",fn,expr());
                    Lua::VM               &vm = *this;
                    auto_ptr<equilibrium>  eq = NULL;
                    if('=' == K[1] )
                    {
                        K.skip(1);
                        if(K.size()<=0) throw exception("%s: invalid empty function name in '%s'",fn,expr());
                        eq = new lua_eq(entry,vm,K);
                    }
                    else
                    {
                        eq = new const_equilibrium(entry,vm->eval<double>(K));
                    }
                    eq->load(words[2],lib);
                    (void) use( eq.yield() );

                } break;

                default:
                    throw exception("%s: unhandled first entry '%s' in '%s'",fn, entry(), expr());
            }

        }

    }

}
