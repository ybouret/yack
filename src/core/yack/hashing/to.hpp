

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
        //______________________________________________________________________
        //
        //! function content to integral type
        //______________________________________________________________________
        template <typename T> inline
        T to(function &H) throw() { T res(0); H.get(&res,sizeof(T)); return res; }

        //______________________________________________________________________
        //
        //! integral hash of a block
        //______________________________________________________________________
        template <typename T> inline
        T to(function &H, const void *block_addr, const size_t block_size) throw()
        { T res(0); H.block(&res,sizeof(T),block_addr,block_size); return res; }

        //______________________________________________________________________
        //
        //! integral hash of a msg
        //______________________________________________________________________
        template <typename T> inline
        T to(function &H, const char *msg) throw()
        { T res(0); H.block(&res,sizeof(T),msg); return res; }


        //______________________________________________________________________
        //
        //! integral hash of a buffer
        //______________________________________________________________________
        template <typename T> inline
        T to(function &H, const memory::ro_buffer &buf) throw()
        { T res(0); H.block(&res,sizeof(T),buf); return res; }

        //______________________________________________________________________
        //
        //
        //! convert a hashing::function to a key hasher
        //
        //______________________________________________________________________
        template <typename T, typename FUNCTION>
        class to_key : public FUNCTION
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            inline explicit to_key() throw() : FUNCTION() {} //!< setup
            inline virtual ~to_key() throw() {}              //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! generic hashing
            inline T of(const void *block_addr, const size_t block_size) throw()
            {
                return to<T>(*this,block_addr,block_size);
            }

            //! type dependent convertion
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

        //______________________________________________________________________
        //
        //
        //! key hasher prototype
        //
        //______________________________________________________________________
        template <typename FUNCTION>
        class to_hkey : public to_key<size_t,FUNCTION>
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            inline explicit to_hkey() throw() : to_key<size_t,FUNCTION>() {} //!< setup
            inline virtual ~to_hkey() throw() {}                             //!< cleanup

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(to_hkey);
        };


    }

}

#endif

