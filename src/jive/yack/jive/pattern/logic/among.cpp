#include "yack/jive/pattern/logic/or.hpp"
#include "yack/jive/pattern/basic/single.hpp"
#include "yack/ptr/auto.hpp"

namespace yack
{
    namespace jive
    {

        pattern *logical::among(const char *buff, size_t size)
        {
            assert(yack_good(buff,size));
            auto_ptr<logical> p = new op_or();
            while(size-- > 0)
            {
                (*p) << new single( *(buff++) );
            }
            return optimize(p.yield());
        }

        pattern * logical:: among(const char *buff)
        {
            return among(buff,buff?0:strlen(buff));
        }

        pattern * logical:: among(const string &s)
        {
            return among(s(),s.size());
        }

    }
}

