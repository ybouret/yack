//! \file

#ifndef YACK_RANDOMIZED_BITS_INCLUDED
#define YACK_RANDOMIZED_BITS_INCLUDED 1

#include "yack/randomized/metrics.hpp"
#include "yack/ptr/arc.hpp"
#include "yack/object.hpp"
#include "yack/counted.hpp"


namespace yack
{

    namespace randomized
    {
        //______________________________________________________________________
        //
        //
        //! bits interface
        //
        //______________________________________________________________________
        class bits : public object, public counted, public metrics
        {
        public:

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~bits() noexcept; //!< cleanup

        protected:
            explicit bits(const uint32_t umax) noexcept; //!< setup

        public:
            //__________________________________________________________________
            //
            // virtual interface
            //__________________________________________________________________
            virtual uint32_t next32() noexcept = 0; //!< uniform in 0..umax

            //__________________________________________________________________
            //
            // non virtual interface
            //__________________________________________________________________
            double operator()(void) noexcept;                   //!< default to double
            size_t leq(const size_t value) noexcept;            //!< floor( ran() * value + 0.5 )
            bool   choice() noexcept;                           //!< ran <= 0.5
            unit_t in(const unit_t a, const unit_t b) noexcept; //!< in [a:b]

            //! double|float|long double|uint[8:16:32:64]_t
            template <typename T> T to() noexcept;

            //! double|float|long double|int[8:16:32:64]_t
            template <typename T> T symm() noexcept;
            
            //! bitwise full construction
            template <typename T> inline
            T full() noexcept
            {
                T ans = 0;
                for(size_t i=sizeof(T)*8;i>0;--i)
                {
                    ans <<= 1;
                    if(choice()) ans |= 1;
                }
                return ans;
            }

            //! bitwise words of exactly nbit
            template <typename T> inline
            T gen(const size_t nbit) noexcept
            {
                assert(nbit<=sizeof(T)*8);
                if(nbit>0)
                {
                    T ans = 1;
                    for(size_t i=nbit;i>1;--i)
                    {
                        ans <<= 1;
                        if(choice()) ans |= 1;
                    }
                    return ans;
                }
                else
                {
                    return T(0);
                }
            }

            //! fill with non zero bytes
            void fill(void *addr, size_t size) noexcept;


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(bits);
        };

        //______________________________________________________________________
        //
        //
        //! alias to shared bits
        //
        //______________________________________________________________________
        typedef arc_ptr<bits> shared_bits;
        


    }

}

#endif


