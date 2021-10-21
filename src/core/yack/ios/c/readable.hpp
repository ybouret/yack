

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
            virtual ~readable_file() throw();             //!< cleanup
            explicit readable_file(const cstdin_t &);     //!< link to stdin
            explicit readable_file(const char *filename); //!< open readable file

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            bool   get(char &C);                       //!< return false if feof, throw if ferror
            size_t get(void *addr, const size_t size); //!< size>0, throw on error


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(readable_file);
        };


    }
}


#endif
