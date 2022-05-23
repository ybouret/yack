
#include "yack/gfx/image/format/bmp.hpp"
#include "yack/gfx/rgb.hpp"
#include "yack/ios/ocstream.hpp"
#include "yack/exception.hpp"
//#include "yack/color/rgba/from-rgba.hpp"

namespace yack
{
    namespace graphic
    {
        const char bmp_format:: clid[] = "BMP";

        bmp_format:: bmp_format() :
        format(clid, "(bmp)&")
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

        pixmap<rgba> bmp_format:: load(const string &filename, const options *) const
        {
            throw exception("%s: load('%s') not implemented",clid,filename());
        }

        void bmp_format:: saveBMP(const string        &filename,
                                  const bitmap        &image,
                                  rgba               (*cproc)(const void *)) const
        {

            //assert(cproc.d==image.d);
            assert(NULL!=cproc);
            
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

        

        void bmp_format:: save(const pixmap<rgba> &surface, const string &filename, const options *) const
        {
            saveBMP(filename,surface,NULL);
        }

    }

}

