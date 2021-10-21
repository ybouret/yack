
#include "yack/system/seed.hpp"
#include "yack/system/pid.hpp"
#include <ctime>
#include "yack/hashing/des64.hpp"

namespace yack
{
    uint64_t system_seed:: get64() throw()
    {
        hashing::des64  h;
        static uint64_t m = 0;
        uint32_t lword = h.mix( process_id::get() );
        uint32_t rword = h.mix( ++m + time(NULL)  );
        h(lword,rword);
        uint64_t ans = lword;
        ans <<= 32;
        ans |= rword;
        return ans;
    }
}
