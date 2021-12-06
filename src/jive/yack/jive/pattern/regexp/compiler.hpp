
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
        class logical;
        typedef ios::xindent<2> rx_indent; //!< alias

        //______________________________________________________________________
        //
        //
        //! internal rx_compiler
        //
        //______________________________________________________________________
        class RXCompiler
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            static const char clid[];       //!< "regexp"
            static const char lparen = '('; //!< alias
            static const char rparen = ')'; //!< alias
            static const char altern = '|'; //!< alternation
            static const char zom    = '*'; //!< zero or more
            static const char oom    = '+'; //!< one or more
            static const char opt    = '?'; //!< optional
            static const char ign    = '&'; //!< ignore case
            static const char lbrack = '['; //!< alias
            static const char rbrack = ']'; //!< alias
            
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! setup all internal pointers
            RXCompiler(const char       *entry,
                        const size_t      count,
                        const dictionary *usrDB) throw();
            ~RXCompiler() throw(); //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            pattern *expression(); //!< create new 'AND' from current position

            
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
            YACK_DISABLE_COPY_AND_ASSIGN(RXCompiler);
            void jokerize(logical &p, const char j) const;
        };
    }

}

#endif
