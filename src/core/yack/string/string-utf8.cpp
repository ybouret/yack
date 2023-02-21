
#define CH utf8
#include "yack/string/utf8.hpp"
#include "string.hxx"


namespace yack
{
    namespace kernel
    {
        template <> const char string<utf8>::clid[] = "utf8_string";
        
        template <>
        const char * string<CH>:: class_uid() const noexcept
        {
            return clid;
        }
    }
}

#include "yack/ios/encoder.hpp"
#include "yack/ios/ostream.hpp"

namespace yack
{
    namespace kernel
    {
        template <>
        size_t string<utf8>:: serialize(ios::ostream &os) const
        {
            size_t nw = ios::encoder::serialize<size_t>(os,chars);
            for(size_t i=1;i<=chars;++i)
            {
                nw += ios::encoder::emit(os,*item[i]);
            }
            return nw;
        }
    }
}
