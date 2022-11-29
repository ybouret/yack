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
        //______________________________________________________________________
        //
        //
        //! node with a prime and its square
        //
        //______________________________________________________________________
        class prime_knot : public natural
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! setup with integral or natural
            template <typename T> inline
            explicit prime_knot(const T &args) :
            natural(args),
            _sq_( natural::squared(*this) ),
            next(0), prev(0)
            {
            }

            //! cleanup
            virtual ~prime_knot() throw();

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! display as natural
            friend std::ostream & operator<<(std::ostream &os, const prime_knot &self)
            {
                os << static_cast<const natural &>(self);
                return os;
            }

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const natural  _sq_; //!< square of this
            prime_knot    *next; //!< for list/pool
            prime_knot    *prev; //!< for list

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(prime_knot);
        };

        //______________________________________________________________________
        //
        //! alias to list of primes
        //______________________________________________________________________
        typedef cxx_list_of<prime_knot> prime_list;


        //______________________________________________________________________
        //
        //
        //! handling natural primes
        //
        //______________________________________________________________________
        class primes : public singleton<primes>
        {
        public:
            //__________________________________________________________________
            //
            // types and definition
            //__________________________________________________________________
            static const at_exit::longevity life_time = 1001; //!< life time
            static const char  * const      call_sign;        //!< call sign

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            const prime_list * operator->() const throw(); //!< access
            const prime_list & operator*()  const throw(); //!< access


            bool is(const natural &n); //!< check


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

