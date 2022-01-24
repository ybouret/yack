#include "yack/hashing/perfect.hpp"

namespace yack
{

    namespace hashing
    {

        perfect::node_type:: node_type(const int c) throw() :
        next(0), prev(0), freq(0), code(c)
        {
        }

        perfect::node_type:: ~node_type() throw()
        {
        }



        perfect:: ~perfect() throw()
        {
        }

        perfect:: perfect() :
        root( new node_type(-1) )
        {
        }
        

    }
}

