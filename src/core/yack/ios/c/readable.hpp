

//! \file
#ifndef YACK_IOS_C_READABLE_FILE_INCLUDED
#define YACK_IOS_C_READABLE_FILE_INCLUDED 1

#include "yack/ios/c/file.hpp"

namespace yack
{
    namespace ios
    {
        //______________________________________________________________________
        //
        //
        //! read-only FILE
        //
        //______________________________________________________________________
        class readable_file : public c_file
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~readable_file() throw();         //!< cleanup
            explicit readable_file(const cstdin_t &); //!< link to stdin

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            bool get(char &C); //!< return if feof, throw if ferror

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(readable_file);
        };


    }
}


#endif
