
#include "yack/ios/serializable.hpp"
#include "yack/ios/encoder.hpp"
#include "yack/type/cstring.h"

namespace yack
{
    namespace ios
    {
        serializable:: serializable() throw()
        {
            
        }

        serializable:: ~serializable() throw()
        {

        }

        size_t  serializable:: save_class_ui(ostream &os) const
        {
            return encoder::serialize(os,class_uid(),yack_cstring_size(class_uid()));
        }

    }

}


