//! \file

#ifndef YACK_ASSOCIATIVE_KEY_WALKER_INCLUDED
#define YACK_ASSOCIATIVE_KEY_WALKER_INCLUDED 1

#include "yack/associative/key-variety.hpp"
#include "yack/memory/buffer/ro.hpp"
#include "yack/type/out-of-reach.hpp"
#include "yack/system/endian.hpp"
#include <cstring>

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! convert a KEY to a tree path
    //
    //__________________________________________________________________________
    template <typename KEY>
    class key_walker
    {
    public:
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline  key_walker() throw() {}
        inline ~key_walker() throw() {}

        //______________________________________________________________________
        //
        //! compute bytes/length for path
        /**
         * if key is integral_type, it is used as local space to swap BE integer
         * since the function is called a search/insert/remove function!!
         *
         */
        //______________________________________________________________________
        inline const uint8_t * operator()(const KEY &key, size_t &len) throw()
        {
            static const int2type< key_variety::cull<KEY>::value > which = {};
            return walk(key,len,which);
        }
        
        
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(key_walker);

        inline const uint8_t * walk(const KEY &key, size_t &len, const key_variety::memory_buffer &) throw()
        {
            len = key.measure();
            return static_cast<const uint8_t *>(key.ro_addr());
        }
        
        inline const uint8_t * walk(const KEY &key, size_t &len, const key_variety::integral_type &) throw()
        {
            len          = sizeof(KEY);
            return static_cast<const uint8_t *>(endian::BEaddr((void*)&key,sizeof(KEY)));
        }

        inline const uint8_t * walk(const KEY key, size_t &len, const key_variety::legacy_string &) throw()
        {
            len = key?0:strlen(key);
            return coerce_cast<uint8_t>(key);
        }
    };
}
#endif
