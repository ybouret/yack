
#include "yack/system/seed.hpp"
#include "yack/system/pid.hpp"
#include "yack/hashing/sha1.hpp"
#include "yack/hashing/to.hpp"
#include <ctime>

namespace yack
{
    namespace
    {
        struct seed_stamp
        {
            process_id::type seed_proc;
            time_t           seed_time;
            size_t           seed_word;

            inline seed_stamp * next() throw()
            {
                seed_time = time(NULL);
                seed_word++;
                return this;
            }

        };
    }

    uint64_t system_seed:: get64() throw()
    {

        static hashing::sha1 hash;
        static seed_stamp    mark = { process_id::get(), 0, 0 };
        return hashing::to<uint64_t>(hash,mark.next(),sizeof(mark));
    }
}
