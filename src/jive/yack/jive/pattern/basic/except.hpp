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
            explicit except(const uint8_t) noexcept; //!< setup code
            virtual ~except()              noexcept; //!< cleanup

            //__________________________________________________________________
            //
            // pattern interface
            //__________________________________________________________________
            //! accept is one char not matching code
            virtual bool     accept(YACK_JIVE_PATTERN_ARGS) const;
            virtual bool     strong() const;               //!< always strong
            virtual void     firsts(first_bytes  &) const; //!< all but code
            virtual void     encode(ios::ostream &) const; //!< graphviz
            virtual pattern *clone()                const; //!< clone
            virtual bool     is_univocal()  const noexcept; //!< false
            

            //__________________________________________________________________
            //
            // serializable interface
            //__________________________________________________________________
            virtual const char *class_uid() const noexcept;       //!< clid
            virtual size_t      serialize(ios::ostream &) const; //!< mark+code

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            bool is_same_than(const except *other) const noexcept;            //!< same code
            friend std::ostream & operator<<(std::ostream &,const except &); //!< display

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
