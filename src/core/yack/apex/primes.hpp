//! \file

#ifndef YACK_APEX_PRIMES_INCLUDED
#define YACK_APEX_PRIMES_INCLUDED 1

#include "yack/apex/natural.hpp"
#include "yack/singleton.hpp"

namespace yack
{
    namespace apex
    {

        class primes : public singleton<primes>
        {
        public:
            static const at_exit::longevity life_time = 1001; //!< life time
            static const char  * const      call_sign;        //!< call sign

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(primes);
            friend class singleton<primes>;
            explicit primes();
            virtual ~primes() throw();
        };
    }
}


#endif

