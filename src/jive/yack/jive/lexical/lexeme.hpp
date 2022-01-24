//! \file

#ifndef YACK_JIVE_LEXEME_INCLUDED
#define YACK_JIVE_LEXEME_INCLUDED 1

#include "yack/jive/token.hpp"

namespace yack
{
    namespace jive
    {

        //______________________________________________________________________
        //
        //
        //! a lexeme is created by a scanner
        //
        //______________________________________________________________________
        class lexeme : public context
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit lexeme(const tag     &,
                            const context &,
                            const size_t) throw(); //!< setup from directive uuid and context
            virtual ~lexeme()             throw(); //!< cleanup
            
            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! full display: ctx+uuid+data
            friend std::ostream & operator<<(std::ostream &, const lexeme &);
            

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const tag    name;  //!< from directive
            const size_t indx;  //!< index
            lexeme      *next;  //!< for list/pool
            lexeme      *prev;  //!< for list/pool
            token        data;  //!< from scanner

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(lexeme);
        };

        //______________________________________________________________________
        //
        //
        //! lexemes
        //
        //______________________________________________________________________
        typedef cxx_list_of<lexeme> lexemes;

    }

}

#endif
