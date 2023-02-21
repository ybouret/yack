
#include "yack/gfx/image/format/tiff-stack.hpp"
#include "yack/gfx/image/format/tiff++.hpp"
#include "yack/exception.hpp"

namespace yack
{
    namespace graphic
    {

        tiff_stack:: ~tiff_stack() noexcept
        {
        }

        static inline
        void load_stack(const string &filename,
                        pixmap<rgba> *tgt,
                        const size_t  num)
        {
            image::itiff  inp(filename);
            for(size_t i=0;i<num;++i,++tgt)
            {
                inp.set_directory(i);
                if(! inp.load(*tgt) ) throw exception("cannot load #%u of '%s'", unsigned(i), filename());
            }
        }

        tiff_stack:: tiff_stack(const string &filename) :
        pixmaps<rgba>(image::itiff::width_of(filename),
                      image::itiff::height_of(filename),
                      image::itiff::directories_of(filename))
        {
            load_stack(filename,&(*this)[1],size());
        }


        tiff_stack:: tiff_stack(const char *filename) :
        pixmaps<rgba>(image::itiff::width_of(filename),
                      image::itiff::height_of(filename),
                      image::itiff::directories_of(filename))
        {
            const string _(filename);
            load_stack(_,&(*this)[1],size());
        }

        

    }

}

