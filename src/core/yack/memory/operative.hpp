

//! \file

#ifndef YACK_MEMORY_OWNERSHIP_INCLUDED
#define YACK_MEMORY_OWNERSHIP_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{

    namespace memory
    {
        //______________________________________________________________________
        //
        //! helper to create/delete assembly of types in memory area
        //______________________________________________________________________
        class operative
        {
        public:
            typedef void (*procedure)(void*); //!< create/delete

            //! create objects
            /**
             \param entry base address
             \param count number of objects
             \param width sizeof(object)
             \param start construct object procedure
             \param clear destruct object procedure (no-throw!)
             */
            explicit operative(void        *entry,
                               const size_t count,
                               const size_t width,
                               procedure    start,
                               procedure    clear);

            //! destruct all objects
            virtual ~operative() throw();

            //! no-throw swap
            void     swap_with(operative &op) throw();

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(operative);
            void            *base;
            size_t           live;
            const size_t     step; //!< sizeof item
            const procedure  kill;

            void zap() throw();
        };

    }

}

#endif
