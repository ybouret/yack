//! \file

#ifndef YACK_HASH_COMPOUND_INCLUDED
#define YACK_HASH_COMPOUND_INCLUDED 1

#include "yack/container/associative.hpp"
#include "yack/data/hash/table.hpp"
#include "yack/associative/hash/key-hasher.hpp"

#include "yack/container/iterator/linked.hpp"


namespace yack
{
    //! default load factor
#define YACK_HASH_COMPOUND_LOAD_FACTOR 4

    //__________________________________________________________________________
    //
    //
    //! generic data for hash_map/hash_set
    //
    //__________________________________________________________________________
    template <
    typename KEY,
    typename T,
    typename NODE,
    typename KEY_HASHER,
	class    ASSOCIATIVE
    >
    class hash_compound : public ASSOCIATIVE
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        YACK_DECL_ARGS_(T,type);                           //!< aliases
        YACK_DECL_ARGS(KEY,key_type);                      //!< aliases
        typedef kernel::hash_table<KEY,NODE> table_type;   //!< alias
        typedef pool_of<NODE>                zpool_type;   //!< alias


        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        inline virtual       ~hash_compound() noexcept { release(); } //!< cleanup

        //! active objects
        inline virtual size_t size()    const noexcept { return (*table).size; }

        //! reserve nodes within pool and table
        inline virtual void reserve(size_t n)
        {
            for(size_t i=n;i>0;--i) zpool.store( object::zacquire<NODE>() );
            table.reserve(n);

            // TODO: adjust in case of failure

        }

        //! for container
        inline virtual size_t available() const noexcept
        {
            return zpool.size;
        }


        //! for container
        inline virtual size_t capacity() const noexcept
        {
            return zpool.size+(*table).size;
        }

        //! associative<KEY,T> interface: search
        inline virtual const_type *search(param_key_type key) const noexcept
        {
            const NODE *node = table.search(hash(key),key);
            return node ? & (**node) : NULL;
        }

        //! associative<KEY,T> interface: remove
        inline virtual bool remove(param_key_type  key) noexcept
        {
            return table.remove(hash(key),key,zpool,quit);
        }

        //! free user objects, keep memory
        inline virtual void free() noexcept
        {
            table.free_with(zpool,quit);
        }

        //! trim superfluous nodes
        inline void trim() noexcept
        {
            table.trim();
            while(zpool.size) object::zrelease( zpool.query() );
        }

        //! release all resources but hash_table
        inline virtual void release() noexcept
        {
            free();
            trim();
        }

        //! info, query load factor
        inline size_t load_factor() const noexcept { return table.average_load(); }


    protected:
        //! setup
        inline explicit hash_compound() noexcept : ASSOCIATIVE(), hash(), table(), zpool(), adjusted(true) {}

        //! auto-adjust, no-throw but with marker
        inline void     safe_adjust() noexcept
        {
            coerce(adjusted) = false;
            try
            {
                table.adjust_for(YACK_HASH_COMPOUND_LOAD_FACTOR);
                coerce(adjusted) = true;
            }
            catch(...) {}
        }

        //! node cleanup
        static NODE *quit(NODE *node) noexcept
        {
            return out_of_reach::naught( destructed(node) );
        }


        
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(hash_compound);

        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
    public:
        mutable KEY_HASHER  hash;   //!< KEY hasher
    protected:
        table_type          table;  //!< low-level table
        zpool_type          zpool;  //!< memory cache

    public:
        const bool adjusted;        //!< table matches the load factor
        
    };


}

#endif

