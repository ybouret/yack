
#include "yack/gfx/format/png.hpp"
#include "yack/gfx/memory.hpp"
#include "yack/exception.hpp"
#include "yack/memory/embed.hpp"

#include "png.h"

namespace yack
{
    namespace graphic
    {

        namespace
        {
            class PNG_Mem
            {
            public:
                const size_t w;
                const size_t h;
                const size_t d;
                const size_t s;
                png_bytep   *row;

                inline ~PNG_Mem() throw()
                {
                    static memory::allocator &mgr = memory_allocator::location();
                    mgr.release(wksp,wlen);
                }

                inline PNG_Mem(const size_t width,
                               const size_t height,
                               const size_t depth) :
                w(width),
                h(height),
                d(depth),
                s(width*depth),
                row(0),
                wksp(0),
                wlen(0)
                {
                    static memory::allocator &mgr = memory_allocator::instance();
                    uint8_t *data  = 0;
                    {
                        memory::embed emb[] =
                        {
                            memory::embed(row,height),
                            memory::embed(data,height*s)
                        };
                        wksp = YACK_MEMORY_EMBED(emb,mgr,wlen);
                    }
                    for(size_t j=0;j<h;++j,data+=s)
                    {
                        row[j] = data;
                    }
                }


            private:
                YACK_DISABLE_COPY_AND_ASSIGN(PNG_Mem);
                void         *wksp;
                size_t        wlen;
            };

        }

    }
}
