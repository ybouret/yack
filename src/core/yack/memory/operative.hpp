

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
            typedef void (*on_init)(void*,const void *); //!< create/delete
            typedef void (*on_quit)(void *);

            //! create objects
            /**
             \param entry base address
             \param count number of objects
             \param width sizeof(object)
             \param start construct object procedure
             \param clear destruct object procedure (no-throw!)
             \param param user's argument to setup
             */
            explicit operative(void        *entry,
                               const size_t count,
                               const size_t width,
                               on_init      start,
                               on_quit      clear,
                               const void  *param);

            //! destruct all objects
            virtual ~operative() throw();

            //! no-throw swap
            void     swap_with(operative &op) throw();

            //! construct helper
            template <typename T> static inline void init(void *addr, const void *) { new (addr) T(); }

            //! construct helper
            template <typename T, typename U>
            static inline void init1(void *addr, const void *args) { new (addr) T( *static_cast<const U*>(args) ); }


            //! destruct helper
            template <typename T> static inline void quit(void *addr) throw() { static_cast<T*>(addr)->~T(); }

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(operative);
            void            *base; //!< base memory
            size_t           live; //!< internal count
            const size_t     step; //!< sizeof item
            const on_quit    kill; //!< destruct procedure

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
            operative(entry,count,sizeof(T), init<T>, quit<T>,NULL)
            {
            }

            //! setup with one argument
            template <typename U>
            inline explicit operative_of(void *entry, const size_t count, const U &args) :
            operative(entry,count,sizeof(T), init1<T,U>, quit<T>, &args)
            {
            }

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(operative_of);
        };

    }

}

#endif
