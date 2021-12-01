//! \file

#ifndef YACK_JIVE_CONTEXT_INCLUDED
#define YACK_JIVE_CONTEXT_INCLUDED 1

#include "yack/jive/tags.hpp"

namespace yack
{
    namespace jive
    {
        //______________________________________________________________________
        //
        //
        //! context for I/O data
        //
        //______________________________________________________________________
        class context : public tag
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! generic setup
            template <typename LABEL> inline
            explicit context(LABEL &id, const unsigned l, const unsigned c) : tag( tags::make(id) ), line(l), column(c) { }
            virtual ~context() throw();       //!< cleanup
            context(const context &) throw(); //!< no-throw copy

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const unsigned line;     //!< line
            const unsigned column;   //!< column

        private:
            YACK_DISABLE_ASSIGN(context);
        };

    }
}

#endif

