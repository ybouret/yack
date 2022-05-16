
#include "yack/gfx/image/format/tiff++.hpp"
#include "yack/exception.hpp"
#include "tiffio.h"

namespace yack
{
    namespace graphic
    {
        namespace image
        {

            tiffxx:: ~tiffxx() throw()
            {
                assert(handle);
                TIFFClose( static_cast<TIFF *>(handle) );
                handle = 0;
            }

            tiffxx:: tiffxx(const string &filename, const char *mode) :
            handle( TIFFOpen( filename(), mode ) )
            {
                if(!handle) throw exception("TIFFOpen(%s,%s)", filename(), mode);
            }
        }

        namespace image
        {
            itiff:: itiff(const string &filename) : tiffxx(filename,"r")
            {
            }

            itiff:: ~itiff() throw()
            {
            }

            unit_t itiff:: width()
            {
                uint32_t     w=0;
                TIFFGetField(static_cast<TIFF *>(handle), TIFFTAG_IMAGEWIDTH,  &w);
                return w;
            }

            unit_t itiff:: height()
            {
                uint32_t     h=0;
                TIFFGetField(static_cast<TIFF *>(handle), TIFFTAG_IMAGELENGTH,  &h);
                return h;
            }



            unit_t itiff:: width_of(const string &filename)
            {
                itiff fp(filename);
                return fp.width();
            }


            unit_t itiff:: height_of(const string &filename)
            {
                itiff fp(filename);
                return fp.height();
            }

            size_t itiff:: directories_of(const string &filename)
            {
                itiff fp(filename);
                return fp.count_directories();
            }

            size_t itiff:: count_directories()
            {
                TIFF      *tif = static_cast<TIFF *>(handle);
                size_t     nd = 0;
                do {
                    ++nd;
                } while( TIFFReadDirectory(tif) );
                return nd;
            }

            static inline
            void expand(pixmap<rgba> &img) throw()
            {
                const unit_t h   = img.h;
                const unit_t w   = img.w;
                unit_t       jlo = 0;
                unit_t       jhi = h;

                for(unit_t count=h/2;count>0;--count)
                {
                    pixrow<rgba> &rlo = img(jlo++);
                    pixrow<rgba> &rhi = img(--jhi);
                    rgba *lo          = &rlo(0);
                    rgba *hi          = &rhi(0);
                    for(size_t i=w;i>0;--i)
                    {
                        const uint32_t lo32 = *(uint32_t *)lo;
                        const uint32_t hi32 = *(uint32_t *)hi;
                        *(lo++) = rgba( TIFFGetR(hi32), TIFFGetG(hi32), TIFFGetB(hi32), TIFFGetA(hi32));
                        *(hi++) = rgba( TIFFGetR(lo32), TIFFGetG(lo32), TIFFGetB(lo32), TIFFGetA(lo32));
                    }
                }
            }

            bool itiff:: load(pixmap<rgba> &img)
            {
                uint32_t    *raster =  coerce_cast<uint32_t>( &img(0)(0) );
                if( 1 == TIFFReadRGBAImage(static_cast<TIFF *>(handle), img.w, img.h, raster, 0))
                {
                    expand(img);
                    return true;
                }
                else
                {
                    return false;
                }
            }

            void itiff:: set_directory(const size_t n)
            {
                if( 1 != TIFFSetDirectory(static_cast<TIFF *>(handle),n))
                {
                    throw exception("TIFFSetDirectory(n=%u) failure", unsigned(n));
                }
            }

        }

        namespace image
        {
            otiff:: otiff(const string &filename) : tiffxx(filename,"w")
            {
            }

            otiff:: ~otiff() throw()
            {
            }

        }

    }

}

