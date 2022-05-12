
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

