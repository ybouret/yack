
#include "yack/ios/serializer/cfile.hpp"
#include "yack/ios/ocstream.hpp"

namespace yack
{
    namespace ios
    {
        namespace serializer
        {
            size_t cfile:: save(const char         *filename,
                                const serializable &object,
                                const bool          append)
            {
                ios::ocstream fp(filename,append);
                return object.serialize(fp);
            }
            
            size_t cfile:: save(const string       &filename,
                                const serializable &object,
                                const bool          append)
            {
                ios::ocstream fp(filename,append);
                return object.serialize(fp);
            }
        }
        
    }
}

