//! \file

#ifndef YACK_RANDOMIZED_METRICS_INCLUDED
#define YACK_RANDOMIZED_METRICS_INCLUDED 1

#include "yack/type/utils.hpp"
#include "yack/ios/fmt/hexa.hpp"

namespace yack
{
    namespace randomized
    {

        template <typename T>
        class u32_to
        {
        public:
            static const unsigned    bits; //!< for word_type to real conversion
            static const uint32_t    maxi; //!< values in 2^bits - 1
            typedef T   (u32_to<T>::*proc)(const uint32_t) const;

            inline ~u32_to() throw() {}

            inline u32_to(const uint32_t umax) throw() :
            user_maxi(umax),
            compacted(user_maxi>maxi),
            unit_deno(0),
            unit_proc(0)
            {
                setup();
            }

            const uint32_t  user_maxi;
            const bool      compacted;
            const T         unit_deno;
            proc            unit_proc;

            inline T unit(const uint32_t u) const
            {
                return ((*this).*unit_proc)(u);
            }



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

        //______________________________________________________________________
        //
        //
        //! base class for metrics
        //
        //______________________________________________________________________
        class metrics
        {
        public:
            explicit metrics(const uint32_t umax) throw();
            virtual ~metrics() throw();

            const uint32_t            m; //!< maxi
            const u32_to<float>       f; //!< float       metrics
            const u32_to<double>      d; //!< double      metrics
            const u32_to<long double> l; //!< long double metrics

            template <typename T> T unit(const uint32_t u) const throw();
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(metrics);

        };



        

    }
}

#endif

