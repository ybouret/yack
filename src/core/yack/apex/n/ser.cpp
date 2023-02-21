#include "yack/apex/natural.hpp"
#include "yack/ios/encoder.hpp"
#include "yack/ios/decoder.hpp"
#include "yack/arith/align.hpp"
#include "yack/system/exception.hpp"

#include <cerrno>

#include <iostream>

namespace yack
{
    namespace apex
    {

        const char natural:: clid[] = "apn";
        
        const char * natural:: class_uid() const noexcept
        {
            return clid;
        }
        
        size_t natural:: serialize(ios::ostream &fp) const
        {
            const readable<uint8_t> &self = *this;
            return ios::encoder::serialize(fp,self);
        }

        natural natural:: construct(ios::istream &fp, size_t &cumul)
        {
            size_t  sz = 0;
            size_t  nr = ios::decoder::construct<size_t>(fp,sz,"apn::bytes");
            natural ans = 0;

            if(sz>0)
            {
                const size_t nw = YACK_ALIGN_ON(word_size,sz)/word_size;
                natural      res(nw,as_capacity);
                res.bytes = sz;
                res.words = nw;
                assert( res.size() == sz);
                for(size_t i=1;i<=sz;++i)
                {
                    uint8_t &b = coerce(res[i]);
                    nr += ios::decoder::read<uint8_t>(fp,b);
                }
                res.update();
                if(sz!=res.size())
                {
                    throw libc::exception(EIO,"invalid apn bytes");
                }
                res.xch(ans);
            }

            cumul += nr;
            return ans;
        }
    }

}


