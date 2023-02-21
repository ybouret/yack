

//! \file
#ifndef YACK_HASHING_TO_INCLUDED
#define YACK_HASHING_TO_INCLUDED 1


#include "yack/hashing/function.hpp"
#include "yack/associative/key-variety.hpp"

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
        T to(function &H) noexcept { T res(0); H.get(&res,sizeof(T)); return res; }

        //______________________________________________________________________
        //
        //! integral hash of a block
        //______________________________________________________________________
        template <typename T> inline
        T to(function &H, const void *block_addr, const size_t block_size) noexcept
        { T res(0); H.block(&res,sizeof(T),block_addr,block_size); return res; }

        //______________________________________________________________________
        //
        //! integral hash of a msg
        //______________________________________________________________________
        template <typename T> inline
        T to(function &H, const char *msg) noexcept
        { T res(0); H.block(&res,sizeof(T),msg); return res; }


        //______________________________________________________________________
        //
        //! integral hash of a buffer
        //______________________________________________________________________
        template <typename T> inline
        T to(function &H, const memory::ro_buffer &buf) noexcept
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
            inline explicit to_key() noexcept : FUNCTION() {} //!< setup
            inline virtual ~to_key() noexcept {}              //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            

            //! type dependent convertion
            template <typename U> inline
            T operator()(const U &obj) noexcept
            {
                static const int2type< key_variety::cull<U>::value > which = {};
                return compute<U>(obj,which);
            }
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(to_key);
            template <typename U>
            inline T compute( const U &buf, const key_variety::memory_buffer & ) noexcept
            {
                return to<T>(*this,buf);
            }

            template <typename U>
            inline T compute( const U &buf, const key_variety::integral_type &) noexcept
            {
                return to<T>(*this,&buf,sizeof(U));
            }
            
            template <typename U>
            inline T compute(const U &buf, const key_variety::legacy_string &) noexcept
            {
                return to<T>(*this,buf);
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
            inline explicit to_hkey() noexcept : to_key<size_t,FUNCTION>() {} //!< setup
            inline virtual ~to_hkey() noexcept {}                             //!< cleanup

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(to_hkey);
        };


    }

}

#endif

