//! \file

#ifndef YACK_JIVE_PATTERN_BASIC_EXCEPT_INCLUDED
#define YACK_JIVE_PATTERN_BASIC_EXCEPT_INCLUDED 1

#include "yack/jive/pattern.hpp"
#include "yack/type/fourcc.h"

namespace yack
{
    namespace jive
    {

        //______________________________________________________________________
        //
        //
        //! except one byte matching
        //
        //______________________________________________________________________
        class except : public pattern
        {
        public:
            //__________________________________________________________________
            //
            // types and definition
            //__________________________________________________________________
            static const uint32_t mark = YACK_FOURCC('N','O','T',':'); //!< mark
            static const char     clid[];                              //!< "except"

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit except(const uint8_t) throw(); //!< setup code
            virtual ~except()              throw(); //!< cleanup

            //__________________________________________________________________
            //
            // pattern interface
            //__________________________________________________________________
            //! accept is one char not matching code
            virtual bool accept(YACK_JIVE_PATTERN_ARGS) const;


            virtual bool strong() const;               //!< always strong
            virtual void firsts(first_bytes  &) const; //!< all but code
            virtual void encode(ios::ostream &) const; //!< graphviz


            //__________________________________________________________________
            //
            // serializable interface
            //__________________________________________________________________
            virtual const char *class_uid() const throw();       //!< clid
            virtual size_t      serialize(ios::ostream &) const; //!< mark+code

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            bool is_same_than(const except *other) const throw(); //!< same code

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const uint8_t code; //!< matching code
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(except);
        };
    }

}

#endif
