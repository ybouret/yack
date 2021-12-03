
#include "yack/jive/pattern/logic/and.hpp"
#include "yack/jive/pattern/basic/single.hpp"
#include "yack/ptr/auto.hpp"

namespace yack
{
    namespace jive
    {

        pattern *logical:: equal(const char *buff, size_t size)
        {
            assert(yack_good(buff,size));
            auto_ptr<logical> p = new op_and();
            while(size-- > 0)
            {
                (*p) << new single( *(buff++) );
            }
            return optimize(p.yield());
        }

        pattern * logical:: equal(const char *buff)
        {
            return equal(buff,buff?0:strlen(buff));
        }

        pattern * logical:: equal(const string &s)
        {
            return equal(s(),s.size());
        }

    }
}

