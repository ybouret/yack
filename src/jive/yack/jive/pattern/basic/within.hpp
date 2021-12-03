//! \file

#ifndef YACK_JIVE_PATTERN_BASIC_WITHIN_INCLUDED
#define YACK_JIVE_PATTERN_BASIC_WITHIN_INCLUDED 1

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
        class within : public pattern
        {
        public:
            //__________________________________________________________________
            //
            // types and definition
            //__________________________________________________________________
            static const uint32_t mark = YACK_FOURCC('[','I','N',']'); //!< mark
            static const char     clid[];                              //!< "single"

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit within(const uint8_t, const uint8_t) throw(); //!< setup lower/upper
            virtual ~within()                             throw(); //!< cleanup

            //__________________________________________________________________
            //
            // pattern interface
            //__________________________________________________________________
            virtual bool accept(YACK_JIVE_PATTERN_ARGS) const; //!< accept is one char within range
            virtual bool strong() const;                       //!< always strong
            virtual void firsts(first_bytes  &) const;         //!< one domain
            virtual void encode(ios::ostream &) const;         //!< graphviz

            //__________________________________________________________________
            //
            // serializable interfacer
            //__________________________________________________________________
            virtual const char *class_uid() const throw();       //!< clid
            virtual size_t      serialize(ios::ostream &) const; //!< mark+lower and upper

            //__________________________________________________________________
            //
            // method
            //__________________________________________________________________
            bool is_same_than(const within *other) const throw(); //!< same boundaries


            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const uint8_t lower; //!< lower code
            const uint8_t upper; //!< upper code
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(within);
        };
    }

}

#endif
