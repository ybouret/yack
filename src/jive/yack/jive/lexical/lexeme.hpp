//! \file

#ifndef YACK_LEXEME_INCLUDED
#define YACK_LEXEME_INCLUDED 1

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
            explicit lexeme(const tag     &who,
                            const context &ctx) throw(); //!< setup from context
            virtual ~lexeme()                   throw(); //!< cleanup

            friend std::ostream & operator<<(std::ostream &, const lexeme &);


            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const tag uuid;
            token     data;
            lexeme   *next; //!< for list/pool
            lexeme   *prev; //!< for list/pool

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
