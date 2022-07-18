//! \file

#ifndef YACK_NET_INIT_INCLUDED
#define YACK_NET_INIT_INCLUDED 1

#include "yack/setup.hpp"
#include "yack/string/fwd.hpp"

namespace yack
{

    namespace net
    {
        class initializer
        {
        public:
            virtual ~initializer() throw();

        protected:
            explicit initializer();
            string   get_host_name() const;

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(initializer);
        };
    }

}



#endif

