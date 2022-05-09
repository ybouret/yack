
#include "yack/gfx/format/options.hpp"
#include "yack/string/tokenizer.hpp"
#include "yack/string/ops.hpp"
#include "yack/exception.hpp"

namespace yack
{
    namespace graphic
    {
        options:: ~options() throw() {}

        options:: options() throw() : options_db() {}


        options & options:: operator<<(const string &info)
        {
            add(info);
            return *this;
        }

        options & options:: operator<<(const char   *info)
        {
            const string _(info);
            add(_);
            return *this;
        }

        const string * options:: query(const char *key) const throw()
        {
            if(key)
            {
                return this->get_tree().search(key,strlen(key));
            }
            else
            {
                return NULL;
            }
        }

        const string * options:: look_up(const options *opt, const char *key) throw()
        {
            return opt ? opt->query(key) : NULL;
        }


        static inline void optclr(string &s) throw()
        {
            strops::clean_with(" \t", 2, s);
        }

        void options:: add(const string &info)
        {
            static const char fn[] = "graphics::option";
            vector<string> words; tokenizer::split_with(':', words, info);
            const size_t   nopts = words.size();
            for(size_t i=1;i<=nopts;++i)
            {
                const string  &word = words[i];
                tokenizer      tkn(word);
                if(!tkn.find_with('=')) throw exception("%s(no key in '%s')",fn,word());
                string key(tkn.token(),tkn.units());
                optclr(key);
                std::cerr << "key=[" << key << "]" << std::endl;
                string val(tkn.after());
                optclr(val);
                std::cerr << "val=[" << val << "]" << std::endl;
                const string *pVal = search(key);
                if(pVal)
                {
                    coerce(*pVal) = val;
                }
                else
                {
                    if(!insert(key,val)) throw exception("%s(failure to insert [%s]=[%s])",fn,key(),val());
                }
            }

        }

        


    }
}
