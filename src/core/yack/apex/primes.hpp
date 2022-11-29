//! \file

#ifndef YACK_APEX_PRIMES_INCLUDED
#define YACK_APEX_PRIMES_INCLUDED 1

#include "yack/apex/natural.hpp"
#include "yack/singleton.hpp"
#include "yack/data/list/cxx.hpp"

namespace yack
{
    namespace apex
    {

        class prime_knot : public natural
        {
        public:
            template <typename T> inline
            explicit prime_knot(const T &args) :
            natural(args), next(0), prev(0)
            {
            }

            virtual ~prime_knot() throw();

            prime_knot *next;
            prime_knot *prev;

            friend std::ostream & operator<<(std::ostream &os, const prime_knot &self)
            {
                os << static_cast<const natural &>(self);
                return os;
            }

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(prime_knot);
        };

        typedef cxx_list_of<prime_knot> prime_list;

        class primes : public singleton<primes>
        {
        public:
            static const at_exit::longevity life_time = 1001; //!< life time
            static const char  * const      call_sign;        //!< call sign

            const prime_list * operator->() const throw();
            const prime_list & operator*()  const throw();


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(primes);
            friend class singleton<primes>;
            explicit primes();
            virtual ~primes() throw();

            prime_list plist;

        };
    }
}


#endif

