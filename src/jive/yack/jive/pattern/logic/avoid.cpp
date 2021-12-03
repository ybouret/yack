#include "yack/jive/pattern/logic/none.hpp"
#include "yack/jive/pattern/basic/single.hpp"
#include "yack/ptr/auto.hpp"

namespace yack
{
    namespace jive
    {

        pattern *logical:: avoid(const char *buff, const size_t size)
        {
            return fill( new op_none(), buff, size );
        }

        pattern * logical:: avoid(const char *buff)
        {
            return avoid(buff,buff?strlen(buff):0);
        }

        pattern * logical:: avoid(const string &s)
        {
            return avoid(s(),s.size());
        }

    }
}

