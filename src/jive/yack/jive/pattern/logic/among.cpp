#include "yack/jive/pattern/logic/or.hpp"
#include "yack/jive/pattern/basic/single.hpp"
#include "yack/ptr/auto.hpp"

namespace yack
{
    namespace jive
    {

        pattern *logical::among(const char *buff, const size_t size)
        {
            return fill( new op_or(), buff, size );
        }

        pattern * logical:: among(const char *buff)
        {
            return among(buff,buff?strlen(buff):0);
        }

        pattern * logical:: among(const string &s)
        {
            return among(s(),s.size());
        }

    }
}

