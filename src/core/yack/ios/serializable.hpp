//! \file

#ifndef YACK_IOS_SERIALIZABLE_INCLUDED
#define YACK_IOS_SERIALIZABLE_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{
    namespace ios
    {
        class ostream;

        //______________________________________________________________________
        //
        //
        //! base class
        //
        //______________________________________________________________________
        class serializable
        {
        public:
            virtual ~serializable() throw();               //!< cleanup
            virtual size_t serialize(ostream &) const = 0; //!< as portable format, return written bytes

        protected:
            explicit serializable() throw(); //!< setup

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(serializable);
        };
    }
}

#endif
