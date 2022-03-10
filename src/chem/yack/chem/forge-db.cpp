
#include "yack/chem/forge.hpp"
#include "yack/chem/equilibria.hpp"
#include "yack/string/tokenizer.hpp"
#include "yack/string/ops.hpp"

namespace yack
{
    namespace chemical
    {
        void equilibria:: operator()(library &lib, const string &data)
        {
            static forge &mgr = forge::instance();

            vector<string> word;
            tokenizer::split_with(':',word,data);
            for(size_t i=word.size();i>0;--i)
            {
                
            }
        }

    }

}
