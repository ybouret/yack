//! \file

#ifndef YACK_COMPILED_BUFFER
#define YACK_COMPILED_BUFFER 1

#include "yack/type/out-of-reach.hpp"
#include "yack/arith/align.hpp"
#include "yack/type/args.hpp"

namespace yack
{

    namespace low_level
    {
        //! common action
        struct compiled_buffer
        {
            //! throw if required>capacity
            static void  check_overflow(const size_t required,
                                        const size_t capacity);
        };
    }

    //__________________________________________________________________________
    //
    //
    //! buffer allocated during compilation
    //
    //__________________________________________________________________________
    template <typename T, const size_t N>
    class compiled_buffer
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        YACK_DECL_ARGS_(T,type);                          //!< aliases
        static const size_t type_size = sizeof(type);     //!< alias
        static const size_t wksp_size = type_size * N;    //!< minimal workspace size
        static const bool   versatile = false;            //!< is not versatile
        static const size_t num_items = N;                //!< number of available itemds

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline  compiled_buffer() noexcept      : workspace(0), wksp() { on();  } //!< default
        inline  compiled_buffer(const size_t n) : workspace(0), wksp() { on(n); } //!< ok if n<=N
        inline ~compiled_buffer() noexcept                             { off(); } //!< cleanup


        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
        mutable_type *workspace; //!< first item address

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(compiled_buffer);
        void *wksp[ YACK_WORDS_GEQ(wksp_size) ];

        inline void on() noexcept {
            YACK_STATIC_ZSET(wksp);
            workspace = coerce_cast<mutable_type>(&wksp[0]);
        }

        inline void on(const size_t required)
        {
            low_level::compiled_buffer::check_overflow(required,N);
            on();
        }

        inline void off() noexcept {
            YACK_STATIC_ZSET(wksp);
            workspace = 0;
        }


    };

}

#endif
