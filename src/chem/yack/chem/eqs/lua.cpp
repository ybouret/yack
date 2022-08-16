
#include "yack/chem/eqs/lua.hpp"
#include "yack/string/tokenizer.hpp"
#include "yack/string/ops.hpp"
#include "yack/chem/library.hpp"
#include "yack/system/imported.hpp"
#include "yack/lua++/function.hpp"

namespace yack
{
    namespace chemical
    {

      
        
        luaEquilibria:: ~luaEquilibria() throw()
        {
        }

        luaEquilibria:: luaEquilibria() : equilibria(), Lua::VM( new Lua::State() )
        {
        }
        
        static inline bool is_empty(const string &word) throw()
        {
            return word.size()<=0;
        }
        
        namespace {
            
            class luaEquilibrium : public equilibrium
            {
            public:
                inline virtual ~luaEquilibrium() throw()
                {
                }
                
                explicit luaEquilibrium(const string &uid,
                                        const size_t  idx,
                                        Lua::VM      &Kvm,
                                        const string &Kid) :
                equilibrium(uid,idx),
                K_(Kvm,Kid)
                {
                }
                
            private:
                YACK_DISABLE_COPY_AND_ASSIGN(luaEquilibrium);
                mutable Lua::Function<double> K_;
                
                virtual double getK(const double t) const
                {
                    return K_(t);
                }
            };
            
        }
        
#define YACK_NEW_EQ(EXPR) static_cast<equilibrium *>(new EXPR)
        
        void luaEquilibria:: operator()(library      &lib,
                                        const string &info)
        {
            // prepare input
            vector<string> word;
            tokenizer::split_with(':',word,info);
            for(size_t i=word.size();i>0;--i)
            {
                string &curr = word[i];
                strops::strip_with(" \t",2,curr);
            }
            word.remove_if(is_empty);
            
            // parse info
            while(word.size())
            {
                string     &first = word[1]; assert(first.size()>0);
                const char  ch    = first[1];
                
                // checking species only
                if( isalpha(ch) )
                {
                    lib << first;
                    word.pop_front();
                    continue;
                }
                
                // checking equilibrium
                if( '@' == ch )
                {
                    first.skip(1);
                    create_eq_for(lib,word);
                    continue;
                }
                
                // checking database
                if( '#' == ch )
                {
                    first.skip(1);
                    insert_eq_for(lib,first);
                    word.pop_front();
                    continue;
                }
                
                throw imported::exception(clid,"invalid '%s'", first());
            }
        
        }
        
        void luaEquilibria:: create_eq_for(library &lib, vector<string> &word)
        {
            assert(word.size()>=3);
            Lua::State &lvm   = **this;
            string     &first = word[1];
           
            const string &eid = first;
            if(eid.size()<=0) throw imported::exception(clid,"empty equilibrium name");
            if(word.size()<3) throw imported::exception(clid,"missing components/constant for <%s>", eid());
            
            const string &Kxp = word[3]; assert(Kxp.size()>0);
            const size_t  idx = size()+1;
            equilibrium  *pEq = isdigit(Kxp[1]) ? YACK_NEW_EQ( const_equilibrium(eid,idx,lvm.eval<double>(Kxp)) ) : YACK_NEW_EQ( luaEquilibrium(eid,idx,*this,Kxp) );
            equilibrium  &usr = use(pEq);
            usr(lib,word[2]);
            
            word.pop_front();
            word.pop_front();
            word.pop_front();
        }

        
        
        
        
        void luaEquilibria:: operator()(library &lib, const char *info)
        {
            const string _(info);
            (*this)(lib,_);
            
        }
        

        
    }

}

#include "yack/chem/designer/eq-info.hpp"
#include "yack/jive/pattern/matching.hpp"
namespace yack
{
    namespace chemical
    {
        void luaEquilibria:: insert_eq_for(library      &lib,
                                           const string &rx)
        {

            jive::matching matches(rx);
            size_t         found = 0;
            vector<string> word(3,as_capacity);
            for(size_t i=0;i<nucleus::eq_info::count;++i)
            {
                const nucleus::eq_info &ctx = nucleus::eq_info::table[i];
                const string            eid = ctx.name;
                if(matches.exactly(eid))
                {
                    word << eid;
                    word << ctx.data;
                    word << ctx.kxpr;
                    create_eq_for(lib,word);
                    ++found;
                }
            }
            if(!found) throw imported::exception(clid,"no equilibrium matchin '%s'", rx());

        }
    }
}
