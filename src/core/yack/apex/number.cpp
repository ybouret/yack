
#include "yack/apex/number.hpp"

namespace yack
{

    namespace apex
    {

        uint64_t number:: add_ticks = 0;
        uint64_t number:: sub_ticks = 0;

        size_t number:: add_count = 0;
        size_t number:: sub_count = 0;

        void number::reset_tracking() throw()
        {
            add_ticks = 0; add_count = 0;
            sub_ticks = 0; sub_count = 0;
        }


        number:: number() throw()
        {
        }

        number:: ~number() throw()
        {
        }
        

    }

}

