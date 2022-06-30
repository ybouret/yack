//! \file

#ifndef YACK_CONCURRENT_JNODE_INCLUDED
#define YACK_CONCURRENT_JNODE_INCLUDED 1

#include "yack/concurrent/queue.hpp"


namespace yack
{

    namespace concurrent
    {
        //______________________________________________________________________
        //
        //
        //! Alive/Zombie node
        //
        //______________________________________________________________________
        class jNode
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            jNode(const job_type &, const job_uuid); //!< setup
            ~jNode() throw();                        //!< cleanup

            //! build with host/method
            template <typename OBJECT_POINTER, typename METHOD_POINTER> inline
            jNode(OBJECT_POINTER o, METHOD_POINTER m, const job_uuid I) : next(0), prev(0), uuid(I), call(o,m) {}

            //! build with functionoid
            template <typename FUNCTION> inline
            jNode(FUNCTION &func, const job_uuid I, const type2type<FUNCTION> &) : next(0), prev(0), uuid(I), call(func) {}

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            jNode         *next; //!< for pool/list
            jNode         *prev; //!< for list
            const job_uuid uuid; //!< uuid for job
            const job_type call; //!< callable functor

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(jNode);
        };
    }

}


#endif
