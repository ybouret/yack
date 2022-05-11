#include "yack/gfx/format.hpp"
#include "yack/type/before-returning.hpp"
#include "yack/fs/vfs.hpp"

namespace yack
{
    namespace graphic
    {
        format:: ~format() throw()
        {
        }

        format:: format(const char *id, const char *xp) :
        name(id),
        ext(xp)
        {
        }

        bool format:: matches(const string &filename) const
        {
            jive::matching                     &choice = ext;
            const before_returning<jive::token> ensure(choice, & jive::token::release, true );
            const string file_ext = vfs::get_extension(filename);
            return choice.exactly(file_ext);
        }

    }

}

