
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

        static inline void bmp_out32(ios::ostream &fp, const unsigned size)
        {
            fp.write( static_cast<uint8_t>( (size) % 256) );
            fp.write( static_cast<uint8_t>( (size / 256) % 256) );
            fp.write( static_cast<uint8_t>( (size / 65536) % 256) );
            fp.write( static_cast<uint8_t>( (size / 16777216)) );

        }

        void bmp_format:: save(const string        &filename,
                               const bitmap        &image,
                               color::data_to_rgba &cproc) const
        {

            assert(cproc.d==image.d);

            // write header
            ios::ocstream fp(filename);

            const unsigned nx = image.w;
            const unsigned ny = image.h;
            
            /* Header 10 bytes */
            fp.write('B');
            fp.write('M');
            const unsigned samples = nx*ny*3;
            bmp_out32(fp,samples+14+40);
            fp.write(0); fp.write(0);
            fp.write(0); fp.write(0);

            /* Offset to image data */
            fp.write(14+40); fp.write(0); fp.write(0); fp.write(0);

            /* Information header 40 bytes */
            fp.write(0x28); fp.write(0); fp.write(0); fp.write(0);
            bmp_out32(fp,nx);
            bmp_out32(fp,ny);
            fp.write(1);  fp.write(0); /* One plane */
            fp.write(24); fp.write(0); /* 24 bits */
            
            /* Compression type == 0 */
            fp.write(0); fp.write(0); fp.write(0); fp.write(0);
            bmp_out32(fp,samples);
            fp.write(1); fp.write(0); fp.write(0); fp.write(0);
            fp.write(1); fp.write(0); fp.write(0); fp.write(0);
            fp.write(0); fp.write(0); fp.write(0); fp.write(0); /* No palette */
            fp.write(0); fp.write(0); fp.write(0); fp.write(0);

            // write binary data
            for(unit_t j=0;j<image.h;++j)
            {
                const bitrow  &brow = image.get_row(j);
                const uint8_t *curr = brow.scanline();
                for(unit_t i=0;i<image.w;++i,curr += image.d)
                {
                    const rgba c = cproc(curr);
                    fp.write(0xff);
                    fp.write(0xff);
                    fp.write(0xff);

                    //fp.write(c.b);
                    //fp.write(c.g);
                    //fp.write(c.r);
                }
            }

        }
    }

}

