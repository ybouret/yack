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
        //! base class for serializable class
        //
        //______________________________________________________________________
        class serializable
        {
        public:
            //__________________________________________________________________
            //
            // virtual interface
            //__________________________________________________________________
            virtual size_t      serialize(ostream &) const         = 0; //!< as portable format, return written bytes
            virtual const char *class_uid()          const throw() = 0; //!< class Unique IDentifier

            //__________________________________________________________________
            //
            // non-virtual interface
            //__________________________________________________________________
            size_t  save_class_ui(ostream &) const; //!< emit strlen(class_uid) and class_uid

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~serializable() throw(); //!< cleanup
        protected:
            explicit serializable() throw(); //!< setup

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(serializable);
        };
    }
}

#endif
