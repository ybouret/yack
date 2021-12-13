//! \file

#ifndef YACK_RANDOMIZED_U32_TO_INCLUDED
#define YACK_RANDOMIZED_U32_TO_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{
    namespace randomized
    {
        //______________________________________________________________________
        //
        //
        //! u32 to real
        //
        //______________________________________________________________________
        template <typename T>
        class u32_to
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            static const unsigned    bits; //!< for word_type to real conversion
            static const uint32_t    maxi; //!< values in 2^bits - 1
            typedef T   (u32_to<T>::*proc)(const uint32_t) const; //!< alias

            //__________________________________________________________________
            //
            //C++
            //__________________________________________________________________
            inline ~u32_to() throw() {} //!< cleanup

            //! setup
            inline u32_to(const uint32_t umax) throw() :
            user_maxi(umax),
            compacted(user_maxi>maxi),
            unit_deno(0),
            unit_proc(0)
            {
                setup();
            }

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            
            //! call to transform 0..user_maxi into ]0:1[
            inline T unit(const uint32_t u) const throw()
            {
                return ((*this).*unit_proc)(u);
            }
            
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const uint32_t  user_maxi; //!< user maxi value
            const bool      compacted; //!< user_maxi>maxi
            const T         unit_deno; //!< pre-computed denominator
            proc            unit_proc; //!< pre-computed method

         


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(u32_to);

            inline void setup() throw()
            {
                static const T one(1);
                if(compacted)
                {
                    // limited by maxi
                    coerce(unit_deno) = static_cast<T>(maxi)+one;
                    unit_proc         = & u32_to<T>::unit_proc_compacted;
                }
                else
                {
                    // only limited by user's maxi
                    coerce(unit_deno) = static_cast<T>(user_maxi)+one;
                    unit_proc         = & u32_to<T>::unit_proc_all_range;
                }
            }

            inline T unit_proc_all_range(const uint32_t u) const throw()
            {
                static const T half(0.5);
                assert(user_maxi<=maxi);
                assert(u<=user_maxi);
                return (static_cast<T>(u)+half)/unit_deno;
            }

            inline T unit_proc_compacted(const uint32_t u) const throw()
            {
                static const T half(0.5);
                assert(user_maxi>maxi);
                assert(u<=user_maxi);
                const uint32_t U = static_cast<uint32_t>( (uint64_t(u) * uint64_t(maxi))/uint64_t(user_maxi) );
                return (half+static_cast<T>(U))/unit_deno;
            }

        };

    }

}

#endif

