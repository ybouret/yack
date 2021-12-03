
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
            static const uint32_t mark = YACK_FOURCC('C','H','R',':'); //!< mark
            static const char     clid[];                              //!< "single"

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit single(const uint8_t) throw(); //!< setup code
            virtual ~single()              throw(); //!< cleanup

            //__________________________________________________________________
            //
            // pattern interface
            //__________________________________________________________________
            //! accept is one char matching code
            virtual bool accept(YACK_JIVE_PATTERN_ARGS) const;

            //! always strong
            virtual bool strong() const;

            //! one domain
            virtual void firsts(first_bytes &) const;

            //! graphviz
            virtual void   encode(ios::ostream &) const;

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
            bool is_same_than(const single *other) const throw(); //!< same code

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const uint8_t code; //!< matching code
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(single);
        };
    }

}

#endif
