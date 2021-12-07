#include "yack/jive/pattern/regexp/compiler.hpp"
#include "yack/jive/pattern/posix.hpp"
#include "yack/exception.hpp"

namespace yack
{
    namespace jive
    {
        pattern *RXCompiler:: posix()
        {
            static const tags::pattern_factory & db = tags::instance().pdb;
            assert(curr[-1]==':');
            assert(curr[-2]==lbrack);

            const char *org  = curr;
            const char *next = org+1;
            while(next<last)
            {
                if(':'==curr[0]&&rbrack==curr[1])
                {
                    // get id
                    const string id(org,curr-org);
                    YACK_JIVE_PRINTLN(RXIndent(deep) << "<posix id='" << id << "'>");

                    // search for it
                    const tags::pattern_creator *mk = db.search(id);
                    if(!mk) throw exception("%s: unknown [:%s:] in '%s'",clid,id(), expr);

                    // update and return
                    curr += 2;
                    return (*mk)();
                }
                ++curr;
                ++next;
            }
            throw exception("%s: POSIX expression unfinished in '%s'",clid,expr);
        }
    }

}

