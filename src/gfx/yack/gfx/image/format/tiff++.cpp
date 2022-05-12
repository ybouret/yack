
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

            size_t itiff:: directories()
            {
                TIFF      *tif = static_cast<TIFF *>(handle);
                size_t     nd = 0;
                do {
                    ++nd;
                } while( TIFFReadDirectory(tif) );
                return nd;
            }

            bool itiff:: load(pixmap<rgba> &img)
            {
                uint32_t    *raster =  coerce_cast<uint32_t>( &img(0)(0) );
                if (TIFFReadRGBAImage(static_cast<TIFF *>(handle), img.w, img.h, raster, 0))
                {
                    img.vflip();
                    return true;
                }
                else
                {
                    return false;
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

