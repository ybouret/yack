
#include "yack/gfx/format/png.hpp"
#include "yack/gfx/memory.hpp"
#include "yack/sequence/cxx-array.hpp"

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


            class PNG_Common
            {
            public:
                inline virtual ~PNG_Common() throw() {}


                inline void failed(const char *fn) const
                {
                    assert(fn);
                    throw exception("%s failure!",fn);
                }


            protected:
                inline explicit PNG_Common() throw() {}



            private:
                YACK_DISABLE_COPY_AND_ASSIGN(PNG_Common);

            };

            class PNG_Bytes
            {
            public:
                const size_t w;
                const size_t h;

                explicit PNG_Bytes(const size_t bytes_per_row, const size_t rows) :
                w(bytes_per_row),
                h(rows),
                row(NULL),
                wksp(NULL),
                wlen(0)
                {
                    static memory::allocator &mgr = memory_allocator::instance();
                    png_byte *data  = 0;
                    {
                        memory::embed emb[] =
                        {
                            memory::embed(row,h),
                            memory::embed(data,w*h)
                        };
                        wksp = YACK_MEMORY_EMBED(emb,mgr,wlen);
                    }
                    for(size_t j=0;j<h;++j,data+=w)
                    {
                        row[j] = data;
                    }
                }

                virtual ~PNG_Bytes() throw()
                {
                    static memory::allocator &mgr = memory_allocator::location();
                    mgr.release(wksp,wlen);
                }

                inline void operator()(png_structp &png)
                {
                    png_read_image(png,row);
                }

                inline const png_bytep & operator[](const size_t y) throw()
                {
                    assert(y<h);
                    return row[y];
                }

            private:
                png_bytep   *row;
                void        *wksp;
                size_t       wlen;
                YACK_DISABLE_COPY_AND_ASSIGN(PNG_Bytes);
            };

            class PNG_Reader : public PNG_Common, public ios::readable_file
            {
            public:
                png_structp png;
                png_infop   info;

                explicit PNG_Reader(const string &filename) :
                PNG_Common(),
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

                virtual ~PNG_Reader() throw()
                {
                    png_destroy_read_struct(&png, &info, NULL);
                }

                inline void call()
                {
                    png_init_io(png, (FILE *)handle );
                    png_read_info(png, info);

                    const png_uint_32 width      = png_get_image_width(png, info);
                    const png_uint_32 height     = png_get_image_height(png, info);
                    const png_byte    color_type = png_get_color_type(png, info);
                    const png_byte    bit_depth  = png_get_bit_depth(png, info);

                    // Read any color_type into 8bit depth, RGBA format.
                    // See http://www.libpng.org/pub/png/libpng-manual.txt

                    if(bit_depth == 16)
                        png_set_strip_16(png);

                    if(color_type == PNG_COLOR_TYPE_PALETTE)
                        png_set_palette_to_rgb(png);

                    // PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth.
                    if(color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
                        png_set_expand_gray_1_2_4_to_8(png);

                    if(png_get_valid(png, info, PNG_INFO_tRNS))
                        png_set_tRNS_to_alpha(png);

                    // These color_type don't have an alpha channel then fill it with 0xff.
                    if(color_type == PNG_COLOR_TYPE_RGB ||
                       color_type == PNG_COLOR_TYPE_GRAY ||
                       color_type == PNG_COLOR_TYPE_PALETTE)
                        png_set_filler(png, 0xFF, PNG_FILLER_AFTER);

                    if(color_type == PNG_COLOR_TYPE_GRAY ||
                       color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
                        png_set_gray_to_rgb(png);

                    png_read_update_info(png, info);

                    std::cerr << width << "x" << height << " : bpp=" << unsigned(bit_depth) << std::endl;

                    const size_t bytes_per_row = png_get_rowbytes(png,info);

                    std::cerr << "bytes_per_row=" << bytes_per_row << std::endl;

#if 0
                    PNG_Bytes data(bytes_per_row,height);
                    data(png);
#endif

                    pixmap<rgba>                          pxm(width,height);
                    cxx_array<png_bytep,memory_allocator> row(height);
                    for(size_t i=0;i<height;++i)
                    {
                        row[i+1] = (png_byte*)&pxm[i][0];
                    }
                    png_read_image(png,&row[1]);
                }

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(PNG_Reader);
            };






            //class png_writer;
        }



        void png_load(const string &filename)
        {
            PNG_Reader *io = new PNG_Reader(filename);
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
