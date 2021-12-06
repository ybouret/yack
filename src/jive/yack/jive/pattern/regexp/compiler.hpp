
//! \file

#ifndef YACK_JIVE_REGEXP_COMPILER_INCLUDED
#define YACK_JIVE_REGEXP_COMPILER_INCLUDED 1

#include "yack/jive/pattern.hpp"

namespace yack
{
    namespace jive
    {

        class dictionary;

        //! internal rx_compiler
        class rx_compiler
        {
        public:
            static const char clid[];
            
            ~rx_compiler() throw();
            rx_compiler(const char       *entry,
                        const size_t      count,
                        const dictionary *usrDB) throw();

            pattern *new_expression();

            

            const char       * const expr;
            const char       *       curr;
            const char       * const last;
            const dictionary * const dict;
            int                      depth;
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(rx_compiler);

        };
    }

}

#endif
