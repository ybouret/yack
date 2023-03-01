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
        struct compiled_buffer
        {
            static void  check_overflow(const size_t required,
                                        const size_t capacity);
        };
    }

    template <typename T, const size_t N>
    class compiled_buffer
    {
    public:
        YACK_DECL_ARGS_(T,type);
        static const size_t type_size = sizeof(type);
        static const size_t wksp_size = type_size * N;
        static const bool   versatile = false;
        static const size_t num_items = N;

        inline  compiled_buffer() noexcept      : workspace(0), wksp() { on();  }
        inline  compiled_buffer(const size_t n) : workspace(0), wksp() { on(n); }
        inline ~compiled_buffer() noexcept                             { off(); }

        mutable_type *workspace;

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
