#include "yack/raven/qfamily.hpp"
#include "yack/system/imported.hpp"


namespace yack
{
    namespace raven
    {

        qfamily:: ~qfamily() throw()
        {
        }

        void qfamily:: throw_singular_matrix(const size_t ir) const
        {
            throw imported::exception( "raven::qfamily","singular matrix[%u]", unsigned(ir) );
        }

    }

}

