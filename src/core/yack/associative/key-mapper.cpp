#include "yack/associative/key-mapper.hpp"
#include "yack/system/imported.hpp"

namespace yack
{
    
    namespace kernel
    {
        key_mapper:: ~key_mapper() noexcept {}

        key_mapper:: key_mapper() noexcept
        {
        }

        static const char fn[] = "key_mapper";

        void key_mapper:: raise_multiple_target() const
        {
            throw imported::exception(fn,"multiple target key");
        }

        void key_mapper:: raise_multiple_source() const
        {
            throw imported::exception(fn,"multiple source key");
        }
    }
}
