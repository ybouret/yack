//! \file

#ifndef YACK_ASSOCIATIVE_KEY_WALKER_INCLUDED
#define YACK_ASSOCIATIVE_KEY_WALKER_INCLUDED 1

#include "yack/type/conversion.hpp"
#include "yack/memory/buffer/ro.hpp"
#include "yack/type/out-of-reach.hpp"

namespace yack
{
    template <typename KEY>
    class key_walker
    {
    public:
        inline  key_walker() throw() {}
        inline ~key_walker() throw() {}
        
        inline const uint8_t * operator()(const KEY &key, size_t &len) throw()
        {
            static const int2type<YACK_IS_SUPERSUBCLASS(memory::ro_buffer,KEY)> choice = {};
            return walk(key,len,choice);
        }
        
        
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(key_walker);
        inline const uint8_t * walk(const KEY &key, size_t &len, const int2type<true> &) throw()
        {
            len = key.measure();
            return static_cast<const uint8_t *>(key.ro_addr());
        }
        
        inline const uint8_t * walk(const KEY &key, size_t &len, const int2type<false> &) throw()
        {
            len = sizeof(KEY);
            return coerce_cast<uint8_t>(&key);
        }
    };
}
#endif

