
#include "yack/ios/fmt/entity.hpp"

namespace yack
{
    namespace ios
    {
        entity:: ~entity() noexcept
        {
        }

        entity:: entity(const entity &_) :
        name(_.name)
        {
        }

    }
}

namespace yack
{
    namespace ios
    {
        entity:: group::  group() noexcept : maxlen(0) {}
        entity:: group:: ~group() noexcept {}
        entity:: group::  group(const group &g) noexcept : maxlen(g.maxlen) {}

    }

}
