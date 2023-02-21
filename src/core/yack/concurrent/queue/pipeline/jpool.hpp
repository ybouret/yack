//! \file

#ifndef YACK_CONCURRENT_JPOOL_INCLUDED
#define YACK_CONCURRENT_JPOOL_INCLUDED 1

#include "yack/concurrent/queue/pipeline/jnode.hpp"
#include "yack/data/pool.hpp"
#include "yack/container/releasable.hpp"
#include <new>

namespace yack
{

    namespace concurrent
    {
        //______________________________________________________________________
        //
        //
        //! pool of zombies
        //
        //______________________________________________________________________
        class jpool : public releasable
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit jpool() noexcept;
            virtual ~jpool() noexcept;

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void     release() noexcept;             //!< release all memory
            void     store(jnode *alive) noexcept;   //!< store an live node
            jnode   *query(const job_type &);       //!< create a new live node

            //! build a new live node from host/method
            template <typename OBJECT_POINTER, typename METHOD_POINTER>
            jnode   *build(OBJECT_POINTER o, METHOD_POINTER m)
            {
                jnode *node = zget();
                try {  new (node) jnode(o,m,uuid); ++uuid; return node; }
                catch(...) { zput(node); throw;     }
            }

            //! build a new libe node from functionoid
            template <typename FUNCTION>
            jnode *build(FUNCTION &func)
            {
                static const type2type<FUNCTION> hint = {};
                jnode *node = zget();
                try {  new (node) jnode(func,uuid,hint); ++uuid; return node; }
                catch(...) { zput(node); throw;     }
            }

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(jpool);
            jnode *zget();
            void   zput(jnode *zombie) noexcept;
            pool_of<jnode> impl; //!< memory
            job_uuid       uuid; //!< current
        };

    }

}


#endif
