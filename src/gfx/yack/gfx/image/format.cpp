#include "yack/gfx/image/format.hpp"
#include "yack/type/before-returning.hpp"
#include "yack/fs/vfs.hpp"

namespace yack
{
    namespace graphic
    {
        namespace image
        {
            format:: ~format() throw()
            {
            }

            format:: format(const char *id, const char *xp) :
            jive::matching(xp),
            counted(),
            codec(id)
            {
            }

            bool format:: matches(const string &filename)
            {
                jive::matching                     &choice = *this;
                const before_returning<jive::token> ensure(choice, & jive::token::release, true );
                const string file_ext = vfs::get_extension(filename);
                return choice.exactly(file_ext);
            }
        }

    }

}

