
//! \file

#ifndef YACK_DATA_SLIM_BANK_INCLUDED
#define YACK_DATA_SLIM_BANK_INCLUDED 1

#include "yack/data/slim/pool.hpp"
#include "yack/counted.hpp"
#include "yack/ptr/arc.hpp"
#include "yack/lockable.hpp"

namespace yack
{
    namespace concurrent
    {
        class mutex;
    }

    namespace kernel
    {
        //______________________________________________________________________
        //
        //! base class for slim_bank
        //______________________________________________________________________
        class slim_bank :
        public object,
        public counted 
        {
        public:
            //__________________________________________________________________
            //
            // definitions
            //__________________________________________________________________
            static const char * const clid;    //!< "slim_bank"
            static bool              &verbose; //!< on concurrent::mutex::verbose
            
            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            lockable & operator*() throw(); //!< access to internal mutex

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~slim_bank() throw(); //!< cleanup

        protected:
            explicit slim_bank();         //!< setup

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(slim_bank);
            concurrent::mutex *sync;
        };
    }

    //__________________________________________________________________________
    //
    //
    //! shared bank of zombie nodes
    //
    //__________________________________________________________________________
    template <typename NODE>
    class slim_bank :
    public kernel::slim_bank,
    public slim_zpool<NODE>
    {
    public:
        //______________________________________________________________________
        //
        // types
        //______________________________________________________________________
        typedef NODE                  node_type; //!< alias
        typedef slim_zpool<node_type> pool_type; //!< alias
        typedef arc_ptr<slim_bank>    pointer;   //!< alias

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline explicit slim_bank() throw() : kernel::slim_bank(), pool_type()  {}  //!< setup empty
        inline virtual ~slim_bank() throw() { release(); assert(0==this->size); }   //!< cleanup


        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________

        //! interface: releasable
        inline virtual void release() throw() {
            YACK_LOCK(**this);
            while(this->size) node_type::zdelete(this->query());
        }

        //! reserve zombie nodes
        inline void reserve(size_t n) throw() {
            YACK_LOCK(**this);
            while(n-- > 0) this->store( node_type::zcreate() );
        }


        //! zombify and store an alive node
        inline void zstore(node_type *alive) throw() {
            YACK_LOCK(**this);
            zstore_unlocked(alive);
        }

        //! zombify and store an entire list
        inline void zstore(list_of<NODE> &alive) throw()
        {
            YACK_LOCK(**this);
            while(alive.size) zstore_unlocked( alive.pop_back() );
        }

        //! fetch all alive when called by destructor
        virtual void  zfinal(list_of<NODE> &alive) throw()
        {
            YACK_LOCK(**this);
            while(alive.size) zstore_unlocked( alive.pop_back() );
        }

        //! query and existent/new zombie node
        inline node_type *zquery()
        {
            YACK_LOCK(**this);
            return this->size ? this->query() : node_type::zcreate();
        }

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(slim_bank);
        inline void zstore_unlocked(node_type *alive) throw()
        {
            this->store( node_type::zombify(alive) );
        }
    };

}

#endif
