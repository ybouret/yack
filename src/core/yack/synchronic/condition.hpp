//! \file

#ifndef YACK_SYNC_CONDITION_INCLUDED
#define YACK_SYNC_CONDITION_INCLUDED 1

#include "yack/synchronic/primitive.hpp"

namespace yack
{
    namespace synchronic
    {
        namespace quark
        {
            class condition;
        }
        
        //______________________________________________________________________
        //
        //
        //! condition object
        //
        //______________________________________________________________________
        class condition : public primitive
        {
        public:
            explicit condition();          //!< create internal condition
            virtual ~condition() throw();  //!< cleanup
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(condition);
            quark::condition *impl;
        };
    }
}

#endif

