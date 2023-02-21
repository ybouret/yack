

//! \file

#ifndef YACK_JIVE_PATTERN_LOGICAL_NONE_INCLUDED
#define YACK_JIVE_PATTERN_LOGICAL_NONE_INCLUDED 1

#include "yack/jive/pattern/logic/bank.hpp"
#include "yack/type/fourcc.h"

namespace yack
{
    namespace jive
    {
        //______________________________________________________________________
        //
        //
        //! logical NONE
        //
        //______________________________________________________________________
        class op_none : public bank
        {
        public:
            //__________________________________________________________________
            //
            // types and definition
            //__________________________________________________________________
            static const uint32_t mark = YACK_FOURCC('[','!','!',']'); //!< mark
            static const char     clid[];                              //!< "and"

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit op_none() noexcept;
            virtual ~op_none() noexcept;

            //__________________________________________________________________
            //
            // interface
            //__________________________________________________________________
            virtual bool     accept(YACK_JIVE_PATTERN_ARGS) const;
            virtual bool     strong() const;
            virtual void     firsts(first_bytes  &) const; //!< negative collect
            virtual void     encode(ios::ostream &) const; //!< graphviz
            virtual pattern *clone()                const; //!< clone
            virtual bool     is_univocal()  const noexcept; //!< false

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            virtual const char *class_uid() const noexcept; //!< clid

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(op_none);
        };

    }

}

#endif
