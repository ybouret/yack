
#include "yack/ordered/compiled-buffer.hpp"
#include "yack/system/imported.hpp"

namespace yack
{

    namespace low_level
    {


        void  compiled_buffer:: check_overflow(const size_t required,
                                               const size_t capacity)
        {
            if(required>capacity)
                throw imported::exception("compiled_buffer",
                                          "%lu exceeds capacity=%lu",
                                          static_cast<unsigned long>(required),
                                          static_cast<unsigned long>(capacity)
                                          );
        }
    }

}

