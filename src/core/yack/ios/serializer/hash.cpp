
#include "yack/ios/serializer/hash.hpp"
#include "yack/ios/ohstream.hpp"

namespace yack
{
    namespace ios
    {
        namespace serializer
        {
            size_t hash:: run(hashing::function &h, const serializable &obj)
            {
                ohstream fp(h);
                const size_t ans = obj.serialize(fp);
                fp.flush();
                return ans;
            }
        }
    }
}

