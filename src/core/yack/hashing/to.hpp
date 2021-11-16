

//! \file
#ifndef YACK_HASHING_TO_INCLUDED
#define YACK_HASHING_TO_INCLUDED 1


#include "yack/hashing/function.hpp"
#include "yack/type/conversion.hpp"

namespace yack
{

    namespace memory { class ro_buffer; }

    namespace hashing
    {

        template <typename T> inline
        T to(function &H) throw() { T res(0); H.get(&res,sizeof(T)); return res; }


        template <typename T> inline
        T to(function &H, const void *block_addr, const size_t block_size) throw()
        { T res(0); H.block(&res,sizeof(T),block_addr,block_size); return res; }

        template <typename T> inline
        T to(function &H, const char *msg) throw()
        { T res(0); H.block(&res,sizeof(T),msg); return res; }


        template <typename T> inline
        T to(function &H, const memory::ro_buffer &buf) throw()
        { T res(0); H.block(&res,sizeof(T),buf); return res; }

        template <typename T, typename FUNCTION>
        class to_key : public FUNCTION
        {
        public:
            inline explicit to_key() throw() : FUNCTION() {}
            inline virtual ~to_key() throw() {}

            inline T of(const void *block_addr, const size_t block_size) throw()
            {
                return to<T>(*this,block_addr,block_size);
            }

            template <typename U> inline
            T operator()(const U &obj) throw()
            {
                static const int2type<YACK_IS_SUPERSUBCLASS(memory::ro_buffer,U)> choice = {};
                return compute<U>(obj,choice);
            }



        private:
            YACK_DISABLE_COPY_AND_ASSIGN(to_key);
            template <typename U>
            inline T compute( const U &buf, int2type<true> ) throw()
            {
                return of(buf.ro_addr(),buf.measure());
            }

            template <typename U>
            inline T compute( const U &buf, int2type<false> ) throw()
            {
                return of(&buf,sizeof(U));
            }
        };

    }

}

#endif

