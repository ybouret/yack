
#include "yack/gfx/format/bmp.hpp"
#include "yack/gfx/rgb.hpp"
#include "yack/ios/ocstream.hpp"

namespace yack
{
    namespace graphic
    {
        bmp_format:: bmp_format() :
        format("BMP", "(bmp)&")
        {

        }

        bmp_format:: ~bmp_format() throw()
        {

        }


        static inline void bmp_w32(unsigned char *p,
                                       const int      i) throw()
        {

            p[ 0] = (unsigned char)( i    );
            p[ 1] = (unsigned char)( i>> 8);
            p[ 2] = (unsigned char)( i>>16);
            p[ 3] = (unsigned char)( i>>24);
        }

        void bmp_format:: save(const string        &filename,
                               const bitmap        &image,
                               color::data_to_rgba &cproc) const
        {

            assert(cproc.d==image.d);

            // initial headers
            unsigned char       bmpfileheader[14] = {'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0};
            unsigned char       bmpinfoheader[40] =
            {   40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 24, 0,
                0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0,
                0,  0, 0, 0, 0, 0, 0, 0
            };
            const unsigned char bmppad[4] = {0,0,0,0};

            const int w        = image.w;
            const int h        = image.h;
            const int ppl      = (4-(w*3)%4)%4; //!< pad per line
            //const int filesize = 54 + 3*w*h;
            const int filesize = 54+h*(3*w+ppl);

            // fill headers
            bmp_w32(&bmpfileheader[2],filesize);
            bmp_w32(&bmpinfoheader[ 4],w);
            bmp_w32(&bmpinfoheader[ 8],h);
            
            ios::ocstream fp(filename);
            fp.frame(bmpfileheader,14);
            fp.frame(bmpinfoheader,40);


            // write binary data

            for(unit_t j=image.h;j>0;)
            {
                const bitrow  &row  = image.line(--j);
                const uint8_t *curr = row.addr();
                for(unit_t i=0;i<image.w;++i,curr += image.d)
                {
                    const rgba c = cproc(curr);
                    fp.write(c.b);
                    fp.write(c.g);
                    fp.write(c.r);
                }
                fp.frame(bmppad,ppl);
            }

        }
    }

}

