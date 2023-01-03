

//! \file

#ifndef YACK_DATA_SLIM_ZPOOL_INCLUDED
#define YACK_DATA_SLIM_ZPOOL_INCLUDED 1

#include "yack/container/releasable.hpp"
#include "yack/data/pool.hpp"
#include "yack/data/list.hpp"

namespace yack
{

    //__________________________________________________________________________
    //
    //
    //! interface for a pool of zombie NODEs
    //
    //__________________________________________________________________________
    template <typename NODE>
    class slim_zpool : public pool_of<NODE>, public releasable
    {
    public:
        //______________________________________________________________________
        //
        // interface
        //______________________________________________________________________
        virtual inline void reserve(size_t)                 = 0; //!< reserve zombies
        virtual inline void zstore(NODE *) throw()          = 0; //!< zombify and store one node
        virtual inline void zstore(list_of<NODE> &) throw() = 0; //!< zombify and store an entire list
        virtual NODE       *zquery()                        = 0; //!< query/create a zombie NODE

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________

        //! cleanup
        inline virtual ~slim_zpool() throw() {}

    protected:
        //! setup
        inline explicit slim_zpool() throw() : pool_of<NODE>(), releasable() {}

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(slim_zpool);
    };

}

#endif

