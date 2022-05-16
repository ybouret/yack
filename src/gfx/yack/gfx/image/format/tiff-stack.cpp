
#include "yack/gfx/image/format/tiff-stack.hpp"
#include "yack/gfx/image/format/tiff++.hpp"
#include "yack/exception.hpp"

namespace yack
{
    namespace graphic
    {

        tiff_stack:: ~tiff_stack() throw()
        {
        }

        tiff_stack:: tiff_stack(const string &filename) :
        pixmaps<rgba>(image::itiff::width_of(filename),
                      image::itiff::height_of(filename),
                      image::itiff::directories_of(filename))
        {
            image::itiff  inp(filename);
            pixmap<rgba> *tgt = &(*this)[1];
            for(size_t i=0;i<size();++i,++tgt)
            {
                inp.set_directory(i);
                if(! inp.load(*tgt) ) throw exception("cannot load #%u", unsigned(i));
            }
        }

        

    }

}

