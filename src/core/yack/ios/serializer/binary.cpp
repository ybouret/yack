
#include "yack/ios/serializer/binary.hpp"
#include "yack/ios/osstream.hpp"
#include "yack/exception.hpp"

namespace yack
{
    namespace ios
    {
        namespace serializer
        {

            string to_binary::from(const serializable &obj)
            {
                string   bin;
                size_t   num = 0;
                {
                    osstream fp(bin);
                    num = obj.serialize(fp);
                    fp.flush();
                }
                if(num!=bin.size()) throw exception("to_binary size mismatch");
                return bin;
            }
        }

        string serializable::to_binary() const
        {
            return serializer::to_binary::from(*this);
        }

    }

}


