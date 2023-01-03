
//! \file

#ifndef YACK_DATA_SLIM_POOL_INCLUDED
#define YACK_DATA_SLIM_POOL_INCLUDED 1

#include "yack/data/slim/node.hpp"
#include "yack/data/slim/zpool.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! pool of zombie NODEs
    //
    //__________________________________________________________________________
    template <typename NODE>
    class slim_pool : public slim_zpool<NODE>
    {
    public:
        //______________________________________________________________________
        //
        // types
        //______________________________________________________________________
        typedef NODE                  node_type; //!< alias
        typedef slim_zpool<node_type> pool_type; //!< alias

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline explicit slim_pool() throw() : pool_type()  {} //!< setup empty
        inline virtual ~slim_pool() throw()  { release(); }   //!< cleanup


        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________

        //! interface: releasable
        inline virtual void release() throw() {
            while(this->size) node_type::zrelease(this->query());
        }

        //! reserve zombie nodes
        inline virtual void reserve(size_t n) throw() {
            while(n-- > 0) this->store( node_type::zcreate() );
        }

        //! zombify and store an alive node
        inline virtual void zstore(node_type *alive) throw() {
            this->store( node_type::zombify(alive) );
        }

        //! zombify and store an entire list
        inline virtual void zstore(list_of<NODE> &alive) throw()
        {
            while(alive.size) zstore( alive.pop_back() );
        }

        //! delete all alive, called by destructors
        virtual void  zfinal(list_of<NODE> &alive) throw()
        {
            while(alive.size) delete alive.pop_back();
        }

        //! query and existent/new zombie node
        inline virtual node_type *zquery()
        {
            return this->size ? this->query() : node_type::zcreate();
        }


    private:
        YACK_DISABLE_COPY_AND_ASSIGN(slim_pool);
    };

    //__________________________________________________________________________
    //
    //
    //! pseudo pointer on a slim_pool, to use with manifest
    //
    //__________________________________________________________________________
    template <typename NODE>
    class slim_hook 
    {
    public:
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline  slim_hook() throw() : impl() {} //!< setup
        inline ~slim_hook() throw()          {} //!< cleanup

        //! copy: start a new pool
        inline  slim_hook(const slim_hook &) throw() : impl() {}

        //______________________________________________________________________
        //
        // access
        //______________________________________________________________________
        inline slim_pool<NODE>       & operator*()        throw() { return  impl; } //!< access
        inline const slim_pool<NODE> & operator*()  const throw() { return  impl; } //!< access
        inline slim_pool<NODE>       * operator->()       throw() { return &impl; } //!< access
        inline const slim_pool<NODE> * operator->() const throw() { return &impl; } //!< access

    private:
        slim_pool<NODE> impl;
        YACK_DISABLE_ASSIGN(slim_hook);
    };



}

#endif
