
#include "yack/ios/markup.hpp"
#include <iostream>
#include "yack/ios/ostream.hpp"

namespace yack
{
    namespace ios
    {
        markup:: markup(const bool   &v,
                        std::ostream &os,
                        const char   *pfx_,
                        const char   *tag_) noexcept :
        vrb(v),
        ptr( &os ),
        out( std_ostream ),
        pfx(pfx_),
        tag(tag_)
        {
            assert(pfx);
            assert(tag);
            enter();
        }

        markup:: markup(const bool   &v,
                        ios::ostream &os,
                        const char   *pfx_,
                        const char   *tag_) noexcept :
        vrb(v),
        ptr( &os ),
        out( lib_ostream ),
        pfx(pfx_),
        tag(tag_)
        {
            assert(pfx);
            assert(tag);
            enter();
        }


        markup:: ~markup() noexcept
        {
            leave();
        }

        void markup:: enter() const noexcept
        {
            try {
                if(vrb)
                {
                    switch(out)
                    {
                        case std_ostream:
                            (*static_cast<std::ostream *>(ptr)) << pfx << '<' << tag << '>' << '\n';
                            break;

                        case lib_ostream:
                            (*static_cast<ios::ostream *>(ptr)) << pfx << '<' << tag << '>' << '\n';
                            break;
                    }
                }
            }
            catch(...)
            {
            }
        }

        void markup:: leave() const noexcept
        {
            try {
                if(vrb)
                {
                    switch(out)
                    {
                        case std_ostream:
                            (*static_cast<std::ostream *>(ptr)) << pfx << '<' << tag << '/' << '>' << '\n';
                            break;

                        case lib_ostream:
                            (*static_cast<ios::ostream *>(ptr)) << pfx << '<' << tag << '/' << '>' << '\n';
                            break;
                    }
                }
            }
            catch(...)
            {
            }
        }



    }

}

