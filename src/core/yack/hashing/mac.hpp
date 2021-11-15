
/! \file

#ifndef YACK_HASHING_MAC_INCLUDED
#define YACK_HASHING_MAC_INCLUDED 1


#include "yack/hashing/hmac

namespace yack
{
    namespace hashing
    {

        template <typename FUNCTION>
        class MAC : public function
        {
        public:
            inline explicit MAC();
            FUNCTION H;
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(MAC);
        };

    }

}

#endif
