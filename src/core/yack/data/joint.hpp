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
#include "yack/type/destruct.hpp"
#include "yack/type/default.hpp"

namespace yack
{

#define YACK_JOINT_POOL_PROLOG()   NODE *node = zquery(); try {           //!< helper for node creation
#define YACK_JOINT_POOL_EPILOG() } catch(...) { zstore(node); throw; }    //!< helper for node creation

    //__________________________________________________________________________
    //
    //
    //! pool for sharing nodes
    //
    //__________________________________________________________________________
    template <typename NODE>
    class joint_pool : public object, public counted, public releasable
    {
    public:
        //______________________________________________________________________
        //
        // types
        //______________________________________________________________________
        typedef NODE                node_type; //!< alias
        typedef pool_of<NODE>       pool_type; //!< alias
        typedef arc_ptr<joint_pool> pointer;   //!< alias

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline explicit joint_pool() throw() : object(), counted(), releasable(), door(), pool() {}
        inline virtual ~joint_pool() throw() { empty(); }

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________

        //! releasable interface
        inline virtual void release() throw() { empty(); }

        //! reserve zombie nodes
        inline void         reserve(size_t n) {
            YACK_LOCK(door);
            while(n-- > 0 ) pool.store( object::zacquire<NODE>() );
        }

        //! access
        inline const pool_type & operator*()  const throw() { return  pool; }

        //! access
        inline const pool_type * operator->() const throw() { return &pool; }

        //! query a zombie node
        inline NODE *zquery()
        {
            YACK_LOCK(door);
            return pool.size ? pool.query() : object::zacquire<NODE>();
        }

        //! store a zombie node, unlocked
        inline void zstore_unlocked(NODE *node) throw()
        {
            assert(NULL!=node);
            pool.store( out_of_reach::naught(node) );
        }

        //! store a zombie node, locked
        inline void zstore(NODE *node) throw()
        {
            YACK_LOCK(door);
            zstore_unlocked(node);
        }


        //! create a default alive node
        inline NODE *create() {
            YACK_JOINT_POOL_PROLOG()
            return new (node) NODE();
            YACK_JOINT_POOL_EPILOG()
        }

        //! create a 1-arg alive node
        template <typename T> inline
        NODE *create(const T &args) {
            YACK_JOINT_POOL_PROLOG()
            return new (node) NODE(args);
            YACK_JOINT_POOL_EPILOG()
        }

        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
        concurrent::mutex door; //!< mutex
    private:
        pool_of<NODE>     pool; //!< pool

        inline void empty() throw() {
            YACK_LOCK(door);
            while(pool.size)
                object::zrelease(pool.query());
        }
    };

    //__________________________________________________________________________
    //
    //
    //! list with nodes sharing for re-use
    //
    //__________________________________________________________________________
    template <typename NODE>
    class joint_list : public list_of<NODE>, public releasable
    {
    public:
        //______________________________________________________________________
        //
        // types
        //______________________________________________________________________
        typedef NODE                        node_type; //!< alias
        typedef list_of<node_type>          list_type; //!< alias
        typedef joint_pool<node_type>       bank_type; //!< alias
        typedef typename bank_type::pointer fund_type; //!< alias

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________

        //! setup empty
        inline explicit joint_list(const fund_type &shared) throw() :
        list_type(), releasable(), fund(shared)  {}

        //! cleanup, returning nodes to pool
        inline virtual ~joint_list() throw() { empty(); }

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________

        //! release, returning nodes to pool
        inline virtual void release() throw() { empty(); }

        //! revoke a node
        inline void revoke(NODE *node) throw() {
            assert(NULL!=node);
            fund->zstore( destructed(node) );
        }

        //______________________________________________________________________
        //
        // helpers
        //______________________________________________________________________
        inline  joint_list & operator<<(const default_type_t &)
        {
            this->push_back( fund->create() );
            return *this;
        }

        template <typename T>
        inline joint_list & operator<<(const T &args) {
            this->push_back( fund->create(args) );
            return *this;
        }



        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
        fund_type fund; //!< shared fund of nodes
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(joint_list);

        inline void empty() throw()
        {
            YACK_LOCK(fund->door);
            while(this->size) fund->zstore_unlocked( destructed(this->pop_back()) );

        }
    };



}

#endif

