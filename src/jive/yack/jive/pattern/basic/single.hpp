
//! \file

#ifndef YACK_JIVE_PATTERN_BASIC_SINGLE_INCLUDED
#define YACK_JIVE_PATTERN_BASIC_SINGLE_INCLUDED 1

#include "yack/jive/pattern.hpp"
#include "yack/type/fourcc.h"

namespace yack
{
    namespace jive
    {
        //______________________________________________________________________
        //
        //
        //! single byte matching
        //
        //______________________________________________________________________
        class single : public pattern
        {
        public:
            //__________________________________________________________________
            //
            // types and definition
            //__________________________________________________________________
            static const uint32_t mark = YACK_FOURCC('B','Y','T','E');
            static const char     clid[];

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit single(const uint8_t) throw();
            virtual ~single()              throw();

            //__________________________________________________________________
            //
            // pattern interface
            //__________________________________________________________________
            virtual result      accept(YACK_JIVE_PATTERN_ARGS) const;

            //__________________________________________________________________
            //
            // serializable interfacer
            //__________________________________________________________________
            virtual const char *class_uid() const throw();
            virtual size_t      serialize(ios::ostream &) const;

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const uint8_t code;
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(single);
        };
    }

}

#endif
