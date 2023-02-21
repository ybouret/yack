
#include "yack/gfx/image/codec.hpp"

namespace yack
{
    namespace graphic
    {
        namespace image
        {
            codec:: codec(const char *id) : name(id)
            {
            }

            codec:: ~codec() noexcept
            {
            }

            const string & codec:: key() const noexcept
            {
                return name;
            }

            pixmap<rgba> codec:: load(const char   *filename, const options *opts) const
            {
                const string _(filename);
                return load(_,opts);
            }

            void codec:: save(const pixmap<rgba> &surface, const char *filename, const options *opts) const
            {
                const string _(filename);
                save(surface,_,opts);
            }


        }

    }

}
