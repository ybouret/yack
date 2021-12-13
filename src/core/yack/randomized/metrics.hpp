//! \file

#ifndef YACK_RANDOMIZED_METRICS_INCLUDED
#define YACK_RANDOMIZED_METRICS_INCLUDED 1

#include "yack/arith/base2.hpp"
#include "yack/type/utils.hpp"

namespace yack
{
    namespace randomized
    {

        //______________________________________________________________________
        //
        //
        //! base class for metrics
        //
        //______________________________________________________________________
        class metrics
        {
        public:
            typedef uint32_t       word_type;                     //!< alias
            static const unsigned  word_size = sizeof(word_type); //!< alias for 4
            static const unsigned  word_bits = word_size << 3;    //!< alias for 32
            static const word_type word_unit = 1;                 //!< alias for 1


            template <typename T>
            class from
            {
            public:
                static const unsigned  bits; //!< for word_type to real conversion
                static const word_type maxi; //!< values in 2^bits - 1
                typedef T   (from<T>::*proc)(const word_type);

                ~from() throw() {}

                from(const word_type umax) throw() :
                user_maxi(umax),
                compacted(user_maxi>maxi),
                unit_deno(0),
                unit_proc(0)
                {
                    setup();
                }

                const word_type user_maxi;
                const bool      compacted;
                const T         unit_deno;
                proc            unit_proc;

                T unit(const word_type u)
                {
                    return ((*this).*unit_proc)(u);
                }


            private:
                YACK_DISABLE_COPY_AND_ASSIGN(from);

                inline void setup() throw()
                {
                    static const T one(1);
                    if(compacted)
                    {
                        // limited by maxi
                        coerce(unit_deno) = static_cast<T>(maxi)+one;
                        unit_proc         = & from<T>::unit_proc_compacted;
                    }
                    else
                    {
                        // only limited by user's maxi
                        coerce(unit_deno) = static_cast<T>(user_maxi)+one;
                        unit_proc         = & from<T>::unit_proc_all_range;
                    }
                }

                inline T unit_proc_all_range(const word_type u) throw()
                {
                    static const T half(0.5);
                    assert(user_maxi<=maxi);
                    assert(u<=user_maxi);
                    return (static_cast<T>(u)+half)/unit_deno;
                }

                inline T unit_proc_compacted(const word_type u) throw()
                {
                    static const T half(0.5);
                    assert(user_maxi>maxi);
                    assert(u<=user_maxi);
                    const uint32_t U = static_cast<uint32_t>( (uint64_t(u) * uint64_t(maxi))/uint64_t(user_maxi) );
                    return (half+static_cast<T>(U))/unit_deno;
                }

            };

            explicit metrics(const word_type umax) throw();
            virtual ~metrics() throw();

            const word_type span;



        private:
            YACK_DISABLE_COPY_AND_ASSIGN(metrics);
        };



        

    }
}

#endif

