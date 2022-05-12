
#include "yack/gfx/image/format/tiff.hpp"
#include "yack/exception.hpp"

#include "tiffio.h"

namespace yack
{
    namespace graphic
    {
        const char tiff_format::clid[] = "TIFF";

        tiff_format:: tiff_format() :
        format(clid, "(tiff|tif)&" )
        {

        }

        tiff_format:: ~tiff_format() throw()
        {
        }


        class tiff_file
        {
        public:
            inline virtual ~tiff_file() throw()
            {
                TIFFClose(tif);
                tif = NULL;
            }

            TIFF * operator*() throw() {
                return tif;
            }

        public:
            inline explicit tiff_file(const string &filename, const char *mode) :
            tif( TIFFOpen( filename(), mode) )
            {
                if(!tif) throw exception("TIFFOpen(%s,%s) failure", filename(),mode);
            }

        private:
            TIFF *tif;
            YACK_DISABLE_COPY_AND_ASSIGN(tiff_file);
        };

        class tiff_ifile : public tiff_file
        {
        public:
            inline explicit tiff_ifile(const string &filename) : tiff_file(filename,"r") {}
            inline virtual ~tiff_ifile() throw() {}

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(tiff_ifile);
        };

        class tiff_ofile : public tiff_file
        {
        public:
            inline explicit tiff_ofile(const string &filename) : tiff_file(filename,"w") {}
            inline virtual ~tiff_ofile() throw() {}

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(tiff_ofile);
        };


        size_t tiff_format:: count_directories(const string &filename)
        {
            tiff_ifile tif(filename);
            size_t     nd = 0;
            do {
                ++nd;
            } while( TIFFReadDirectory(*tif) );
            return nd;
        }


        pixmap<rgba> tiff_format:: load(const string &filename, const options *) const
        {
            tiff_ifile tif(filename);
            uint32_t     w=0; TIFFGetField(*tif, TIFFTAG_IMAGEWIDTH,  &w); if(w<=0) throw exception("%s: w=0 in '%s'",clid,filename());
            uint32_t     h=0; TIFFGetField(*tif, TIFFTAG_IMAGELENGTH, &h); if(h<=0) throw exception("%s: h=0 in '%s'",clid,filename());
            pixmap<rgba> img(w,h);
            uint32_t    *raster =  coerce_cast<uint32_t>( &img(0)(0) );
            if (TIFFReadRGBAImage(*tif, w, h, raster, 0))
            {
                img.vflip();
            }
            else
            {
                throw exception("%s: can't ReadRGBAImage '%s'",clid,filename());
            }
            return img;
        }

        void tiff_format:: save(const pixmap<rgba> &img, const string &filename, const options *)
        {
            tiff_ofile tif(filename);
            int compression = COMPRESSION_NONE;
            TIFFSetField(*tif, TIFFTAG_IMAGEWIDTH,  (uint32_t) img.w);
            TIFFSetField(*tif, TIFFTAG_IMAGELENGTH, (uint32_t) img.h);
            TIFFSetField(*tif, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
            TIFFSetField(*tif, TIFFTAG_COMPRESSION, compression);
            TIFFSetField(*tif, TIFFTAG_SAMPLESPERPIXEL, 4);
            TIFFSetField(*tif, TIFFTAG_BITSPERSAMPLE,   8);
            TIFFSetField(*tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
            TIFFSetField(*tif, TIFFTAG_PHOTOMETRIC,  PHOTOMETRIC_RGB);

            for(unit_t j=0;j<img.h;++j)
            {
                if( TIFFWriteScanline(*tif,(void*)(&img(j)(0)),j,0) < 0 )
                {
                    throw exception("Error in TiffWriteScanLine");
                }
            }

        }

        

    }

}

