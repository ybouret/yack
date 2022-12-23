//! \file

#ifndef YACK_DATA_JOINT_INCLUDED
#define YACK_DATA_JOINT_INCLUDED 1

#include "yack/object.hpp"
#include "yack/counted.hpp"
#include "yack/data/list.hpp"
#include "yack/data/pool.hpp"
#include "yack/concurrent/mutex.hpp"
#include "yack/container/releasable.hpp"
#include "yack/type/out-of-reach.hpp"
#include "yack/ptr/arc.hpp"

namespace yack
{

    template <typename NODE>
    class joint_pool : public object, public counted, public releasable
    {
    public:
        typedef NODE                node_type;
        typedef pool_of<NODE>       pool_type;
        typedef arc_ptr<joint_pool> pointer;

        inline explicit joint_pool() throw() : object(), counted(), releasable(), door(), pool() {}
        inline virtual ~joint_pool() throw() { empty(); }

        inline virtual void release() throw() { empty(); }

        inline void         reserve(size_t n) {
            YACK_LOCK(door);
            while(n-- > 0 ) pool.store( object::zacquire<NODE>() );
        }

        inline const pool_type & operator*()  const throw() { return  pool; }
        inline const pool_type * operator->() const throw() { return &pool; }

        //! query a zombie node
        inline NODE *zquery()
        {
            YACK_LOCK(door);
            return pool.size ? pool.query() : object::zacquire<NODE>();
        }

        //! store a zombie node
        inline void zstore_unlocked(NODE *node) throw()
        {
            assert(NULL!=node);
            pool.store( out_of_reach::naught(node) );
        }

        inline void ztore(NODE *node) throw()
        {
            YACK_LOCK(door);
            zstore_unlocked(node);
        }


        concurrent::mutex door;
    private:
        pool_of<NODE>     pool;

        inline void empty() throw() {
            YACK_LOCK(door);
            while(pool.size)
                object::zrelease(pool.query());
        }
    };

    template <typename NODE>
    class joint_list : public list_of<NODE>, public releasable
    {
    public:
        typedef NODE                        node_type;
        typedef list_of<node_type>          list_type;
        typedef joint_pool<node_type>       bank_type;
        typedef typename bank_type::pointer fund_type;

        inline explicit joint_list(const fund_type &shared) throw() :
        list_type(), releasable(), fund(shared), list()  {}

        inline virtual ~joint_list() throw() { empty(); }

        fund_type fund;
    private:
        list_type list;
        YACK_DISABLE_COPY_AND_ASSIGN(joint_list);
        inline void empty() throw()
        {
            YACK_LOCK(fund->door);
        }
    };



}

#endif

