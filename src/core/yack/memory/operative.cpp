
#include "yack/memory/operative.hpp"

namespace yack
{

    namespace memory
    {

        void operative:: zap() noexcept
        {
            uint8_t *addr = static_cast<uint8_t*>(base)+live*step;
            while(live>0)
            {
                addr -= step;
                kill(addr);
                --live;
            }
        }

        operative:: ~operative() noexcept
        {
            zap();
         }

        operative:: operative(void        *entry,
                              const size_t count,
                              const size_t width,
                              on_init      start,
                              on_quit      clear,
                              const void  *param) :
        base(entry),
        live(0),
        step(width),
        kill(clear)
        {
            assert(width>0);
            assert(start);
            assert(clear);
            
            uint8_t *addr = static_cast<uint8_t*>(base);
            try {
                while(live<count)
                {
                    start(addr,param);
                    addr += step;
                    ++live;
                }
            }
            catch(...)
            {
                zap();
                throw;
            }
        }


        void operative:: swap_with(operative &other) noexcept
        {
            YACK_EXCHANGE(base);
            YACK_EXCHANGE(live);
            YACK_EXCHANGE(step);
            YACK_EXCHANGE(kill);
        }


    }

}


