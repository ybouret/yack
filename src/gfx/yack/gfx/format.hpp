//! \file

#ifndef YACK_GFX_FORMAT_INCLUDED
#define YACK_GFX_FORMAT_INCLUDED 1

#include "yack/gfx/article.hpp"
#include "yack/gfx/format/options.hpp"
#include "yack/gfx/pixmap.hpp"
#include "yack/gfx/rgb.hpp"
#include "yack/jive/pattern/matching.hpp"

namespace yack
{
    namespace graphic
    {
        class format : public article
        {
        public:


            bool matches(const string &filename) const;
            const jive::pattern &get_pattern() const throw();
            

            const string name;

            virtual ~format() throw();

        protected:
            explicit format(const char *id, const char *xp);

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(format);
            mutable jive::matching ext;
        };
    }
}

#endif

