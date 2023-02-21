#include "yack/gfx/image/format.hpp"
#include "yack/type/before-returning.hpp"
#include "yack/fs/vfs.hpp"

namespace yack
{
    namespace graphic
    {
        namespace image
        {
            format:: ~format() noexcept
            {
            }

            format:: format(const char *id, const char *xp) :
            codec(id),
            extensions(xp)
            {
            }

            bool format:: matches(const string &filename)
            {
                const before_returning<jive::token> ensure(extensions, & jive::token::release, true );
                const string file_ext = vfs::get_extension(filename);
                //std::cerr << "[" << name << "] testing '" << file_ext << "'" << std::endl;
                return extensions.exactly(file_ext);
            }
        }

    }

}

