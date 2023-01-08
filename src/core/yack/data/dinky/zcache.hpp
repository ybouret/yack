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
        virtual void   zstore(NODE *)  throw()         = 0; //!< store a zombie node
        virtual NODE  *zquery()                        = 0; //!< query a new zombie node
        virtual void   devour(list_of<NODE> &) throw() = 0; //!< turn all alives anto zombies
       
        //______________________________________________________________________
        //
        // virtual: cache management
        //______________________________________________________________________
        virtual size_t stowage() const throw()         = 0; //!< available
        virtual void   reserve(size_t)                 = 0; //!< reserve zombies
        
        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        
        //! make a live dinky_node
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
        
        //! turn live into zombie
        inline static NODE *turn(NODE *live) throw()
        {
            assert(live);
            return static_cast<NODE *>(out_of_reach:: naught( destructed(live) ));
        }
        
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline virtual ~zcache() throw() {} //!< cleanup
        
    protected:
        inline explicit zcache() throw() : releasable() {} //!< setup/
        
        
        
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(zcache);
        
        
    };
}

#endif

