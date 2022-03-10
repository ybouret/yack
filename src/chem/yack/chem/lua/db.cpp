
#include "yack/chem/library.hpp"
#include "yack/chem/lua/equilibria.hpp"
#include "yack/string/tokenizer.hpp"
#include "yack/string/ops.hpp"
#include "yack/exception.hpp"
#include "yack/jive/pattern/matching.hpp"
#include "yack/lua++/function.hpp"

namespace yack
{
    namespace chemical
    {

        static const char fn[] = "chemical::equilibria: ";


        namespace
        {
            struct eq_info
            {
                const char *name;
                const char *data;
                const char *kstr;
            };

            static const eq_info eqdb[] =
            {
#include "equilibria.hxx"
            };

            static const size_t neqs = sizeof(eqdb)/sizeof(eqdb[0]);


            static inline
            void process(library        &lib,
                         lua_equilibria &eqs,
                         string         &expr)
            {
                assert('#'==expr[1]);
                if(expr.skip(1).size()<=0) throw exception("%sempty expression",fn);

                jive::matching matches = expr;
                size_t         found   = 0;

                for(size_t i=0;i<neqs;++i)
                {
                    const eq_info     &info = eqdb[i];
                    const char * const name = info.name; assert(name);
                    if( matches.exactly(name) )
                    {
                        ++found;
                        const double K  = eqs->eval<double>(info.kstr);
                        equilibrium &eq = eqs.create(name,K);
                        eq.parse_with(lib,info.data);
                    }
                }

                if(!found) throw exception("%sno matching '%s'",fn,expr());
            }

            class lua_equilibrium : public equilibrium
            {
            public:
                inline virtual ~lua_equilibrium() throw() {}

                inline explicit lua_equilibrium(const string &uid,
                                                const string &fcn,
                                                const size_t  pos,
                                                const Lua::VM &lvm) :
                equilibrium(uid,pos),
                call(lvm,fcn)
                {
                }


            private:
                YACK_DISABLE_COPY_AND_ASSIGN(lua_equilibrium);
                mutable Lua::Function<double> call;
                inline virtual double getK(const double t) const
                {
                    return call(t);
                }
            };

            static inline
            equilibrium *new_eq(const string &name,
                                const string &kstr,
                                Lua::VM      &lvm,
                                const size_t  pos)
            {
                assert(name.size()>0);
                assert(kstr.size()>0);

                if( isalpha(kstr[1]) )
                {
                    // look for function
                    return new lua_equilibrium(name,kstr,pos,lvm);
                }
                else
                {
                    // constant if numerical
                    return new const_equilibrium(name,pos,lvm->eval<double>(kstr));
                }

            }

        }

        void lua_equilibria:: operator()(library &lib, const char   *data)
        {
            const string _(data);
            (*this)(lib,_);
        }


        static inline bool is_bad_word(string &word)
        {
            strops::clean_with(" \t",2,word);
            return word.size()<=0;
        }




#define LBRACK '['
        void lua_equilibria:: operator()(library &lib, const string &data)
        {
            //------------------------------------------------------------------
            //
            // tokenize data
            //
            //------------------------------------------------------------------
            vector<string> word;   tokenizer::split_with(':',word,data);
            const size_t   nw = word.remove_if(is_bad_word).size();
            if(nw<=0) return;

            //------------------------------------------------------------------
            //
            // get first word
            //
            //------------------------------------------------------------------
            string &kind = word[1]; assert(kind.size()>0);

            switch(kind[1])
            {
                case '#':     // search database
                    if(1!=nw) throw exception("%s%s shouldn't have extra fields",fn,kind());
                    process(lib,*this,kind);
                    return;


                case LBRACK: // list of species
                    for(size_t i=1;i<=nw;++i) lib.parse(word[i]);
                    return;

                default: // construct new equilibrium
                    break;
            }

            if(nw!=3) throw exception("%sdefinition should have 3 fields instead of %u", fn, unsigned(nw) );

            //------------------------------------------------------------------
            //
            // create/insert a new eq
            //
            //------------------------------------------------------------------
            equilibrium &eq = use( new_eq(word[1],word[3],*this,size()+1) );
            eq.parse_with(lib,word[2]);

        }

    }

}
