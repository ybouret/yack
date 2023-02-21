//! \file

#ifndef YACK_GFX_BLOB_INCLUDED
#define YACK_GFX_BLOB_INCLUDED 1

#include "yack/gfx/cnode.hpp"
#include "yack/object.hpp"

namespace yack
{
    namespace graphic
    {
        //______________________________________________________________________
        //
        //
        //! a blob is a list of coordinates
        //
        //______________________________________________________________________
        class blob : public object, public cnodes
        {
        public:
            //------------------------------------------------------------------
            //
            // C++
            //__________________________________________________________________
            explicit blob(const size_t) noexcept; //!< setup with tag
            virtual ~blob()             noexcept; //!< cleanup

            //------------------------------------------------------------------
            //
            // members
            //__________________________________________________________________
            const size_t tag;  //!< identifier>0
            blob        *next; //!< for list
            blob        *prev; //!< for list
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(blob);
        };

       

    }
}

#endif

