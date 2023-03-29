
#include "yack/aqueous/weasel/designer.hpp"
#include "yack/aqueous/weasel/linker.hpp"
#include "yack/jive/pattern/matching.hpp"
#include "yack/string/tokenizer.hpp"
#include "yack/system/imported.hpp"
#include "yack/string/ops.hpp"

namespace yack
{
    namespace aqueous
    {

        namespace weasel
        {
            void linker:: on_rx(const string &rx)
            {
                jive::matching  match(rx);
                size_t          count = 0;
                for(size_t i=0;i<designer::ndb;++i)
                {
                    // find
                    const char *txt =designer::edb[i];
                    tokenizer   tkn(txt);
                    if(!tkn.find_with(':'))
                        throw imported::exception(designer::call_sign,"bad db entry");
                    const string id(tkn.token(),tkn.units());
                    strops::strip_with(" \t", 2, coerce(id));
                    if(match.exactly(id))
                    {
                        ++count;
                        found << string(txt);
                    }
                }
                if(count<=0)
                    throw imported::exception(designer::call_sign,"no equilibrium matching <%s>", rx());
            }

        }

    }

}


