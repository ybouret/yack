//! \file

#ifndef YACK_NETWORK_INCLUDED
#define YACK_NETWORK_INCLUDED 1

#include "yack/singleton.hpp"

namespace yack
{

    //! network
    class network : public singleton<network>
    {
    public:
        static const at_exit::longevity life_time = 3000; //!< for singleton
        static const char               call_sign[];      //!< for singleton

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(network);
        friend class singleton<network>;

        virtual ~network() throw();
        explicit network();
    };

}

#endif


