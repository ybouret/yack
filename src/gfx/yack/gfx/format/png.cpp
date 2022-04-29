
#include "yack/gfx/format/png.hpp"
#include "yack/gfx/memory.hpp"
#include "yack/exception.hpp"
#include "yack/memory/embed.hpp"
#include "yack/string.hpp"
#include "yack/ios/c/readable.hpp"

#include "png.h"

namespace yack
{
    namespace graphic
    {

        namespace
        {


            class png_common
            {
            public:
                inline virtual ~png_common() throw() {}


                inline void failed(const char *fn) const
                {
                    assert(fn);
                    throw exception("%s failure!",fn);
                }


            protected:
                inline explicit png_common() throw() {}



            private:
                YACK_DISABLE_COPY_AND_ASSIGN(png_common);

            };

            class png_reader : public png_common, public ios::readable_file
            {
            public:
                png_structp png;
                png_infop   info;

                explicit png_reader(const string &filename) :
                png_common(),
                ios::readable_file(filename),
                png(NULL),
                info(NULL)
                {
                    png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
                    if(!png) failed("png_create_read_struct");

                    info = png_create_info_struct(png);
                    if(!info)
                    {
                        png_destroy_read_struct(&png,NULL,NULL);
                        failed("png_create_info_struct");
                    }

                }

                virtual ~png_reader() throw()
                {
                    png_destroy_read_struct(&png, &info, NULL);
                }

                inline void call()
                {
                    png_init_io(png, (FILE *)handle );
                    png_read_info(png, info);
                }


            };






            //class png_writer;


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

        void png_load(const string &filename)
        {
            png_reader *io = new png_reader(filename);
            try
            {
                if (setjmp(png_jmpbuf(io->png)))
                {
                    io->failed("png_load");
                }

                io->call();

                delete io;
            }
            catch(...)
            {
                delete io;
                throw;
            }

        }

    }
}
