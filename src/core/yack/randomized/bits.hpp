//! \file

#ifndef YACK_RANDOMIZED_BITS_INCLUDED
#define YACK_RANDOMIZED_BITS_INCLUDED 1

#include "yack/setup.hpp"
#include "yack/arith/base2.hpp"

namespace yack
{

    namespace randomized
    {

        class bits
        {
        public:
            const size_t count;
            const size_t bytes;

            virtual ~bits() throw();

        protected:
            explicit bits(const size_t num_bits) throw();

        public:
            // virtual interface
            virtual double operator()(void) throw() = 0;

            // non virtual interface
            size_t leq(const size_t value) throw();
            bool   choice() throw();


            //! double|float|uint[8:16:32:64]_t
            template <typename T> T to() throw();

            template <typename T> inline
            T full() throw()
            {
                T ans = 0;
                for(size_t i=sizeof(T)*8;i>0;--i)
                {
                    ans <<= 1;
                    if(choice()) ans |= 1;
                }
                return ans;
            }

            template <typename T> inline
            T gen(const size_t nbit) throw()
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
            void fill(void *addr, size_t size) throw();


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(bits);
        };


      



    }

}

#endif


