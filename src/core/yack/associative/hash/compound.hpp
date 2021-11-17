//! \file

#ifndef YACK_HASH_COMPOUND_INCLUDED
#define YACK_HASH_COMPOUND_INCLUDED 1

#include "yack/container/associative.hpp"
#include "yack/data/hash/table.hpp"
#include "yack/associative/hash/key-hasher.hpp"


namespace yack
{

#define YACK_HASH_COMPOUND_LOAD_FACTOR 4

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
        YACK_DECL_ARGS_(T,type);
        YACK_DECL_ARGS(KEY,key_type);
        typedef kernel::hash_table<KEY,NODE> table_type;
        typedef pool_of<NODE>                zpool_type;
        virtual ~hash_compound() throw() {}

        inline virtual size_t size()  const throw() { return (*table).size; }

        inline virtual void reserve(size_t n)
        {
            for(size_t i=n;i>0;--i) zpool.store( object::zacquire<NODE>() );
            table.reserve(n);
        }

        inline virtual const_type *search(param_key_type key) const throw()
        {
            const NODE *node = table.search(hash(key),key);
            return node ? & (**node) : NULL;
        }

        mutable KEY_HASHER  hash;
    protected:
        inline explicit hash_compound() throw() : ASSOCIATIVE(), hash(), table(), zpool(), adjusted(true) {}
        inline void     safe_adjust() throw()
        {
            coerce(adjusted) = false;
            try
            {
                table.adjust_for(YACK_HASH_COMPOUND_LOAD_FACTOR);
                coerce(adjusted) = true;
            }
            catch(...) {}
        }

        static NODE *quit(NODE *node) throw()
        {
            return out_of_reach::naught( destructed(node) );
        }

        table_type          table;
        zpool_type          zpool;
        
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(hash_compound);
        
    public:
        const bool adjusted;
    };


}

#endif

