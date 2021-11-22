
#include "yack/sequence/vector.hpp"
#include "yack/type/cstring.h"

namespace yack
{
    
    namespace kernel
    {
        static const char vector_category[] = "vector.";

        const char *vector_category_build(char *buffer,const size_t length, const char *call_sign) throw()
        {
            assert(NULL!=buffer);
            assert(length>0);
            assert(NULL!=call_sign);
            yack_cstring_msgcpy(buffer,length,vector_category);
            yack_cstring_msgcat(buffer,length,call_sign);
            return buffer;
        }
    }


}
