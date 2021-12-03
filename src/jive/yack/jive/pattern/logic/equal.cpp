
#include "yack/jive/pattern/logic/and.hpp"
#include "yack/jive/pattern/basic/single.hpp"
#include "yack/ptr/auto.hpp"

namespace yack
{
    namespace jive
    {

        pattern *logical:: equal(const char *buff, const size_t size)
        {
            return fill(new op_and(),buff,size);
        }

        pattern * logical:: equal(const char *buff)
        {
            return equal(buff,buff?strlen(buff):0);
        }

        pattern * logical:: equal(const string &s)
        {
            return equal(s(),s.size());
        }

    }
}

