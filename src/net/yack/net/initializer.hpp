//! \file

#ifndef YACK_NET_INIT_INCLUDED
#define YACK_NET_INIT_INCLUDED 1

#include "yack/setup.hpp"
#include "yack/string/fwd.hpp"

namespace yack
{

    namespace net
    {
        //______________________________________________________________________
        //
        //
        //! system initializer
        //
        //______________________________________________________________________
        class initializer
        {
        public:
            virtual ~initializer() noexcept; //!< cleanup

        protected:
            explicit initializer();         //!< setup
            string   get_host_name() const; //!< gethostname

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(initializer);
        };
    }

}



#endif

