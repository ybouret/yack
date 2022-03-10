
#include "yack/chem/forge.hpp"
#include "yack/chem/equilibria.hpp"
#include "yack/string/tokenizer.hpp"
#include "yack/string/ops.hpp"
#include "yack/exception.hpp"

namespace yack
{
    namespace chemical
    {

        void equilibria:: operator()(library &lib, const char   *data)
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

        void equilibria:: operator()(library &lib, const string &data)
        {
            //static forge   &mgr = forge::instance();

            vector<string> word;
            tokenizer::split_with(':',word,data);
            const size_t nw = word.remove_if(is_bad_word).size();
            std::cerr << "#nw=" << nw << std::endl;
            std::cerr << word << std::endl;

            const string &kind = word[1]; assert(kind.size()>0);
            switch(kind[1])
            {
                case '#': // search database
                    throw exception("not implemented");
                    break;

                case '@':    // new equilibria
                    throw exception("not implemented");
                    break;

                case LBRACK: // list of species
                    for(size_t i=1;i<=nw;++i) lib.parse(word[i]);
                    break;
            }







        }

    }

}
