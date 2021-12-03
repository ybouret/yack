//! \file

#ifndef YACK_JIVE_PATTERN_LOGICAL_OR_INCLUDED
#define YACK_JIVE_PATTERN_LOGICAL_OR_INCLUDED 1

#include "yack/jive/pattern/logic/bank.hpp"
#include "yack/type/fourcc.h"

namespace yack
{
    namespace jive
    {
        //______________________________________________________________________
        //
        //
        //! logical OR
        //
        //______________________________________________________________________
        class op_or : public bank
        {
        public:
            //__________________________________________________________________
            //
            // types and definition
            //__________________________________________________________________
            static const uint32_t mark = YACK_FOURCC('[','|','|',']'); //!< mark
            static const char     clid[];                              //!< "and"

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit op_or() throw();
            virtual ~op_or() throw();

            //__________________________________________________________________
            //
            // interface
            //__________________________________________________________________
            virtual bool accept(YACK_JIVE_PATTERN_ARGS) const;
            virtual bool strong() const;
            virtual void firsts(first_bytes &) const;

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            virtual const char *class_uid() const throw(); //!< clid

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(op_or);
        };

    }

}

#endif
