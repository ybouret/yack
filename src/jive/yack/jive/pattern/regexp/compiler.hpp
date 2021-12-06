
//! \file

#ifndef YACK_JIVE_REGEXP_COMPILER_INCLUDED
#define YACK_JIVE_REGEXP_COMPILER_INCLUDED 1

#include "yack/jive/pattern.hpp"
#include "yack/ios/indent.hpp"

namespace yack
{
    namespace jive
    {

        class dictionary;

        typedef ios::xindent<2> rx_indent; //!< alias

        //______________________________________________________________________
        //
        //
        //! internal rx_compiler
        //
        //______________________________________________________________________
        class rx_compiler
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            static const char clid[];       //!< "regexp"
            static const char lparen = '('; //!< alias
            static const char rparen = ')'; //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! setup all internal pointers
            rx_compiler(const char       *entry,
                        const size_t      count,
                        const dictionary *usrDB) throw();
            ~rx_compiler() throw(); //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            pattern *new_expression(); //!< create new 'AND' from current position

            
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const char       * const expr;  //!< original expression
            const char       *       curr;  //!< current position
            const char       * const last;  //!< first invalid position
            const dictionary * const dict;  //!< optional dictionary
            int                      deep;  //!< current depth
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(rx_compiler);

        };
    }

}

#endif
