
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

        const string & entity:: key() const noexcept
        {
            return name;
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

        void entity::group:: update(const entity &ent) noexcept
        {
            coerce(maxlen) = max_of(maxlen,ent.name.size());
        }
    }

}
