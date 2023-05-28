
#include "yack/chem/weasel/designer.hpp"
#include "yack/chem/weasel/linker.hpp"
#include "yack/jive/pattern/matching.hpp"
#include "yack/string/tokenizer.hpp"
#include "yack/system/imported.hpp"
#include "yack/ptr/shared.hpp"
#include "yack/sequence/vector.hpp"
#include "yack/string/ops.hpp"

namespace yack
{
    namespace Chemical
    {
        namespace Weasel
        {
            static const char * const db[] = {
#include "db.hxx"
            };

            static const unsigned known = sizeof(db)/sizeof(db[0]);

            void   Designer:: aliases(Library &lib, LuaEquilibria &eqs)
            {
                typedef shared_ptr<jive::matching> matcher;
                Designer &self = *this;

                //--------------------------------------------------------------
                // create list of matching
                //--------------------------------------------------------------
                vector<matcher> matches;
                
                for(const solo_list<string>::node_type *node=linker->alias.head;node;node=node->next)
                {
                    const string  &rx = **node;
                    std::cerr << "processing <" << rx << ">" << std::endl;
                    const matcher temp = new jive::matching(rx);
                    matches << temp;
                }

                //--------------------------------------------------------------
                // loop ONCE over all known eqs
                //--------------------------------------------------------------
                for(unsigned i=0;i<known;++i)
                {
                    const char *inp = db[i];
                    tokenizer   tkn(inp);
                    if(!tkn.find_with(':')) throw exception(call_sign,"invalid db[%u]='%s'",i,inp);
                    string uid( tkn.token(), tkn.units() );
                    strops::strip_with(" \t",2,uid);

                    for(size_t k=matches.size();k>0;--k)
                    {
                        if(matches[k]->exactly(uid))
                        {
                            self( jive::module::open_data(uid,inp), lib, eqs);
                            break;
                        }
                    }

                }

            }

        }
    }

}


