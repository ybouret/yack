
#include "yack/memory/book.hpp"
#include "yack/type/out-of-reach.hpp"

namespace yack
{

    namespace memory
    {

        book:: ~book() throw()
        {
            //chapter *ch = coerce_cast<chapter>(impl);
        }

        book:: book() throw() //: impl()
        {
            //chapter *ch = static_cast<chapter *>( out_of_reach::zset(impl,sizeof(impl) ) );
        }
      
    }

}
