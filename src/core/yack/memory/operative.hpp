

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
        //
        //! helper to create/delete assembly of types in memory area
        //
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

            //! construct helper
            template <typename T> static inline void init(void *addr) { new (addr) T(); }

            //! destruct helper
            template <typename T> static inline void quit(void *addr) throw() { static_cast<T*>(addr)->~T(); }

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(operative);
            void            *base; //!< base memory
            size_t           live; //!< internal count
            const size_t     step; //!< sizeof item
            const procedure  kill; //!< destruct procedure

            void zap() throw();
        };

        //______________________________________________________________________
        //
        //
        //! parametrized operative
        //
        //______________________________________________________________________
        template <typename T> class operative_of : public operative
        {
        public:
            //! cleanup
            inline virtual ~operative_of() throw() {}

            //! setup
            inline explicit operative_of(void *entry, const size_t count) :
            operative(entry,count,sizeof(T), init<T>, quit<T>)
            {
            }
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(operative_of);
        };

    }

}

#endif
