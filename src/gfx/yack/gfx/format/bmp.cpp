
#include "yack/gfx/format/bmp.hpp"
#include "yack/gfx/rgb.hpp"
#include "yack/ios/ocstream.hpp"

namespace yack
{
    namespace graphic
    {
        bmp_format:: bmp_format()
        {

        }

        bmp_format:: ~bmp_format() throw()
        {

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
            const int filesize = 54 + 3*w*h;
            const int npad     = (4-(w*3)%4)%4;

            // fill headers
            bmpfileheader[ 2] = (unsigned char)(filesize    );
            bmpfileheader[ 3] = (unsigned char)(filesize>> 8);
            bmpfileheader[ 4] = (unsigned char)(filesize>>16);
            bmpfileheader[ 5] = (unsigned char)(filesize>>24);

            bmpinfoheader[ 4] = (unsigned char)(       w    );
            bmpinfoheader[ 5] = (unsigned char)(       w>> 8);
            bmpinfoheader[ 6] = (unsigned char)(       w>>16);
            bmpinfoheader[ 7] = (unsigned char)(       w>>24);

            bmpinfoheader[ 8] = (unsigned char)(       h    );
            bmpinfoheader[ 9] = (unsigned char)(       h>> 8);
            bmpinfoheader[10] = (unsigned char)(       h>>16);
            bmpinfoheader[11] = (unsigned char)(       h>>24);

            //f = fopen("img.bmp","wb");
            //fwrite(bmpfileheader,1,14,f);
            //fwrite(bmpinfoheader,1,40,f);

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
                fp.frame(bmppad,npad);
            }

        }
    }

}

