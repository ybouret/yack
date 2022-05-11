
#include "yack/gfx/image-io.hpp"

namespace yack
{
    namespace graphic
    {

        imageIO:: imageIO(const char *id) : name(id)
        {
        }

        imageIO:: ~imageIO() throw()
        {
        }


        pixmap<rgba> imageIO:: load(const char   *filename, const options *opts) const
        {
            const string _(filename);
            return load(_,opts);
        }


    }

}
