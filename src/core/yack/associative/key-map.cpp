
#include "yack/associative/key-map.hpp"
#include "yack/system/imported.hpp"

namespace yack
{

    namespace kernel
    {
        key_map::  key_map() noexcept {}
        key_map:: ~key_map() noexcept {}
        void key_map:: raise_missing_key() const
        {
            throw imported::exception("key_map","missing key");
        }
    }
}

