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
            explicit jpool() throw();
            virtual ~jpool() throw();

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void     release() throw();             //!< release all memory
            void     store(jNode *alive) throw();   //!< store an live node
            jNode   *query(const job_type &);       //!< create a new live node

            //! build a new live node from host/method
            template <typename OBJECT_POINTER, typename METHOD_POINTER>
            jNode   *build(OBJECT_POINTER o, METHOD_POINTER m)
            {
                jNode *node = zget();
                try {  new (node) jNode(o,m,uuid); ++uuid; return node; }
                catch(...) { zput(node); throw;     }
            }

            //! build a new libe node from functionoid
            template <typename FUNCTION>
            jNode *build(FUNCTION &func)
            {
                static const type2type<FUNCTION> hint = {};
                jNode *node = zget();
                try {  new (node) jNode(func,uuid,hint); ++uuid; return node; }
                catch(...) { zput(node); throw;     }
            }

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(jpool);
            jNode *zget();
            void   zput(jNode *zombie) throw();
            pool_of<jNode> impl; //!< memory
            job_uuid       uuid; //!< current
        };

    }

}


#endif
