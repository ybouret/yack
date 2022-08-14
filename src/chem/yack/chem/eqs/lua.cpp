
#include "yack/chem/eqs/lua.hpp"
#include "yack/string/tokenizer.hpp"
#include "yack/string/ops.hpp"
#include "yack/chem/library.hpp"
#include "yack/system/imported.hpp"

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
            
            std::cerr << "info=" << word << std::endl;
            while(word.size())
            {
                string     &first = word[1]; assert(first.size()>0);
                const char  ch    = first[1];
                if( isalpha(ch) )
                {
                    lib << first;
                    word.pop_front();
                    continue;
                }
                
                throw imported::exception(clid,"invalid '%s'", first());
            }
            
            
            
        }
        
        void luaEquilibria:: operator()(library &lib, const char *info)
        {
            const string _(info);
            (*this)(lib,_);
            
        }
        
        

        
    }

}
