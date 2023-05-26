
//! \file

#ifndef YACK_WEASEL_LINKER_INCLUDED
#define YACK_WEASEL_LINKER_INCLUDED 1


#include "yack/jive/syntax/transcriber.hpp"
#include "yack/jive/syntax/xnode.hpp"

namespace yack
{
    namespace Chemical
    {
        namespace Weasel
        {
            class Linker : public jive::syntax::transcriber
            {
            public:
                enum Sign
                {
                    Plus  =  1,
                    Minus = -1
                };

                explicit Linker();
                virtual ~Linker() noexcept;

                solo_list<string> names;
                solo_list<Sign>   signs;


            private:
                YACK_DISABLE_COPY_AND_ASSIGN(Linker);
                virtual void on_init();
                virtual void on_quit() noexcept;
                void clr() noexcept;

                void on_id(const lexeme &);
                void on_plus(const lexeme &);
                void on_minus(const lexeme &);

                void on_sp(const string &, const size_t );
                void on_weasel(const string &,const size_t);
            };
        }
    }

}

#endif

