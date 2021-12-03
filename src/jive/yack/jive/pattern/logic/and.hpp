
//! \file

#ifndef YACK_JIVE_PATTERN_LOGICAL_AND_INCLUDED
#define YACK_JIVE_PATTERN_LOGICAL_AND_INCLUDED 1

#include "yack/jive/pattern/logic/logical.hpp"
#include "yack/type/fourcc.h"

namespace yack
{
    namespace jive
    {
        //______________________________________________________________________
        //
        //
        //! logical AND
        //
        //______________________________________________________________________
        class op_and : public logical
        {
        public:
            //__________________________________________________________________
            //
            // types and definition
            //__________________________________________________________________
            static const uint32_t mark = YACK_FOURCC('[','&','&',']'); //!< mark
            static const char     clid[];                              //!< "and"

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit op_and() throw();
            virtual ~op_and() throw();

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
            YACK_DISABLE_COPY_AND_ASSIGN(op_and);
        };

    }

}

#endif
