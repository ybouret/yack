
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

        //! query and existent/new zombie node
        inline virtual node_type *zquery()
        {
            return this->size ? this->query() : node_type::zcreate();
        }


    private:
        YACK_DISABLE_COPY_AND_ASSIGN(slim_pool);
    };


}

#endif
