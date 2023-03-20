//! \file

#ifndef YACK_DINKY_ZCACHE_INCLUDED
#define YACK_DINKY_ZCACHE_INCLUDED 1

#include "yack/data/list.hpp"
#include "yack/type/out-of-reach.hpp"
#include "yack/container/releasable.hpp"
#include "yack/type/destruct.hpp"
#include "yack/type/transmogrify.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! base class for cache of zombie NODEs
    //
    //__________________________________________________________________________
    template <typename NODE>
    class zcache : public releasable
    {
    public:
        //______________________________________________________________________
        //
        // virtual: NODE interface
        //______________________________________________________________________
        virtual void   zstore(NODE *)  noexcept         = 0; //!< store a zombie node
        virtual NODE  *zquery()                         = 0; //!< query a new zombie node
        virtual void   devour(list_of<NODE> &) noexcept = 0; //!< turn all alives anto zombies

        //______________________________________________________________________
        //
        // virtual: cache management
        //______________________________________________________________________
        virtual size_t stowage() const noexcept         = 0; //!< available
        virtual void   reserve(size_t)                  = 0; //!< reserve zombies
        
        //______________________________________________________________________
        //
        // non-virtual methods
        //______________________________________________________________________

        //! ingest a live node
        inline void ingest(NODE *live) noexcept
        {
            assert(NULL!=live);
            zstore( turn(live) );
        }

        //! make a live dinly_node, default
        inline NODE *create() {
            NODE *node = zquery();
            try
            {
                return new (node) NODE();
            }
            catch(...)
            {
                zstore(node);
                throw;
            }
        }

        //! make a live dinky_node with one argument
        template <typename U> inline
        NODE *create(const U &args) {
            NODE *node = zquery();
            try
            {
                return new (node) NODE(args,transmogrify);
            }
            catch(...)
            {
                zstore(node);
                throw;
            }
        }

        //! make a live dinky_node with two arguments
        template <typename U, typename V> inline
        NODE *create(const U &u, const V &v) {
            NODE *node = zquery();
            try
            {
                return new (node) NODE(u,v,transmogrify);
            }
            catch(...)
            {
                zstore(node);
                throw;
            }
        }

        //! make a live dinky_node with three arguments
        template <typename U, typename V, typename W> inline
        NODE *create(const U &u, const V &v, const W &w) {
            NODE *node = zquery();
            try
            {
                return new (node) NODE(u,v,w,transmogrify);
            }
            catch(...)
            {
                zstore(node);
                throw;
            }
        }




        //! turn live into zombie
        inline static NODE *turn(NODE *live) noexcept
        {
            assert(live);
            return static_cast<NODE *>(out_of_reach:: naught( destructed(live) ));
        }


        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline virtual ~zcache() noexcept {} //!< cleanup
        
    protected:
        inline explicit zcache() noexcept : releasable() {} //!< setup/
        
        
        
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(zcache);
        
        
    };
}

#endif

