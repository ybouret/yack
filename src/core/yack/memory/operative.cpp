
#include "yack/memory/operative.hpp"

namespace yack
{

    namespace memory
    {

        void operative:: zap() throw()
        {
            uint8_t *addr = static_cast<uint8_t*>(base)+live*step;
            while(live>0)
            {
                addr -= step;
                kill(addr);
                --live;
            }
        }

        operative:: ~operative() throw()
        {
            zap();
         }

        operative:: operative(void        *entry,
                              const size_t count,
                              const size_t width,
                              procedure    start,
                              procedure    clear) :
        base(entry),
        live(0),
        step(width),
        kill(clear)
        {
            assert(entry);
            assert(width>0);
            assert(start);
            assert(clear);

            uint8_t *addr = static_cast<uint8_t*>(base);
            try {
                while(live<count)
                {
                    start(addr);
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


        void operative:: swap_with(operative &op) throw()
        {
            cswap(base,op.base);
            cswap(live,op.live);
            coerce_cswap(step,op.step);
            coerce_cswap(kill,op.kill);
        }


    }

}


