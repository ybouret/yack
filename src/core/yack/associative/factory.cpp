
#include "yack/associative/factory.hpp"
#include "yack/exception.hpp"
#include "yack/system/rtti.hpp"
#include "yack/string.hpp"

namespace yack
{
    namespace kernel
    {

        factory:: ~factory() noexcept
        {
        }

        factory:: factory() noexcept
        {
        }

        void factory:: throw_multiple_key(const std::type_info &tid) const
        {
            static const char * const id = yack::rtti::name(tid)();
            throw exception("multiple key for factory<%s>", id);
        }


        size_t factory::  check_valid_length(const char *key, const std::type_info &tid) const
        {
            static const char * const id = yack::rtti::name(tid)();
            if(NULL==key)
            {
                throw exception("NULL key for for factory<%s>",id);
            }
            const size_t len = strlen(key);
            if(len<=0)
            {
                throw exception("empty key for for factory<%s>",id);
            }
            return len;
        }

        void   factory:: throw_unknown_key(const std::type_info &tid) const
        {
            static const char * const id = yack::rtti::name(tid)();
            throw exception("unknown key for for factory<%s>",id);
        }


    }
}
