
//! \file

#ifndef YACK_MEMORY_BUFFER_IO_INCLUDED
#define YACK_MEMORY_BUFFER_IO_INCLUDED 1

#include "yack/system/endian.hpp"

namespace yack
{
    
    namespace memory
    {
     
        //! I/O ops on buffers
        struct buffer_io
        {
            static inline void shl_or(uint8_t  &x, const uint8_t b) noexcept { x = b; }           //!<  (x <<= 8) |= b
            static inline void shl_or(uint16_t &x, const uint8_t b) noexcept { (x <<= 8) |= b; }  //!<  (x <<= 8) |= b
            static inline void shl_or(uint32_t &x, const uint8_t b) noexcept { (x <<= 8) |= b; }  //!<  (x <<= 8) |= b
            static inline void shl_or(uint64_t &x, const uint8_t b) noexcept { (x <<= 8) |= b; }  //!<  (x <<= 8) |= b

            //! extract next unsigned from flat binary
            template <typename T> static inline
            T get(const void * &addr, size_t &size) noexcept
            {
                T              x = 0;
                size_t         n = sizeof(T);
                const uint8_t *p = static_cast<const uint8_t *>(addr);
                while(n>0&&size>0) {
                    shl_or(x,*(p++));
                    --n;
                    --size;
                }
                addr = p;
                return endian::swap_be(x);
            }
            
        };
    }
    
}

#endif

