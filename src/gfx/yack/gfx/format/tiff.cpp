
#include "yack/gfx/format/tiff.hpp"
#include "yack/exception.hpp"

#include "tiffio.h"

namespace yack
{
    namespace graphic
    {
        const char tiff_format::clid[] = "TIFF";

        tiff_format:: tiff_format() :
        format(clid, "(tif|tiff)&" )
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
    }

}

