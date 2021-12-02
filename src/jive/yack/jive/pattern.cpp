
#include "yack/jive/pattern.hpp"
#include "yack/ios/encoder.hpp"

namespace yack
{
    namespace jive
    {
        pattern:: ~pattern() throw()
        {
        }

        pattern:: pattern(const uint32_t t) throw() :
        uuid(t),
        next(0),
        prev(0),
        self(0)
        {

        }

        void pattern:: I_am(const void *addr) throw()
        {
            *(void **)&self = (void*)addr;
        }

        size_t pattern:: emit_uuid(ios::ostream &fp) const
        {
            return ios::encoder::emit(fp,uuid);
        }

        bool pattern:: feeble() const
        {
            return !strong();
        }
    }

}

