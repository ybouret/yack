

//! \file
#ifndef YACK_IOS_C_READABLE_FILE_INCLUDED
#define YACK_IOS_C_READABLE_FILE_INCLUDED 1

#include "yack/ios/c/file.hpp"

namespace yack
{
    namespace ios
    {

        class readable_file : public c_file
        {
        public:
            virtual ~readable_file() throw();
            explicit readable_file(const cstdin_t &);

            bool get(char &C);

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(readable_file);
        };


    }
}


#endif
