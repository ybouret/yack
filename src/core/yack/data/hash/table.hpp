//! \file

#ifndef YACK_DATA_HASH_TABLE_INCLUDED
#define YACK_DATA_HASH_TABLE_INCLUDED 1

#include "yack/data/list.hpp"
#include "yack/data/hash/node.hpp"
#include "yack/data/pool/cxx.hpp"
#include "yack/type/out-of-reach.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! basic hash_table
    //
    //__________________________________________________________________________
    template <typename KEY, typename NODE>
    class hash_table
    {
    public:
        YACK_DECL_ARGS(KEY,key_type);             //!< aliases
        typedef NODE                   node_type; //!< alias
        typedef list_of<NODE>          list_type; //!< live nodes
        typedef pool_of<NODE>          pool_type; //!< for user to get back node
        typedef hash_node<KEY,NODE>    meta_node; //!< for slots
        typedef list_of<meta_node>     slot_type; //!< slot type
        typedef cxx_pool_of<meta_node> meta_pool; //!< pool of meta nodes
        typedef NODE *               (*quit_proc)(NODE*);


        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________

        //! setup
        inline explicit hash_table() throw() :
        data(), slot(0), slots(1), smask(0), repo(), base()
        {
            slot = &base;
        }

        //! cleanup
        inline virtual ~hash_table() throw()
        {

        }


        //______________________________________________________________________
        //
        //! generic search function, const
        //______________________________________________________________________
        inline const node_type *search(const size_t    hkey,
                                       const_key_type &key) const throw()
        {

            //__________________________________________________________________
            //
            // find slot
            //__________________________________________________________________
            assert(NULL!=slot);
            const slot_type     &load = slot[hkey&smask];

            //__________________________________________________________________
            //
            // scan slot
            //__________________________________________________________________
            for(const meta_node *meta = load.head;meta;meta=meta->next)
            {
                assert(NULL!=meta->node);
                const node_type *node = meta->node;
                //______________________________________________________________
                //
                // two stages comparison
                //______________________________________________________________
                if( (hkey==meta->hkey) && (key==node->key()) )
                {
                    return node;
                }
            }
            return NULL;
        }

        //______________________________________________________________________
        //
        //! generic search function, not const
        //______________________________________________________________________
        inline   node_type *search(const size_t    hkey,
                                   const_key_type &key) throw()
        {

            //__________________________________________________________________
            //
            // find slot
            //__________________________________________________________________
            assert(NULL!=slot);
            slot_type     &load = slot[hkey&smask];

            //__________________________________________________________________
            //
            // scan slot
            //__________________________________________________________________
            for( meta_node *meta = load.head;meta;meta=meta->next)
            {
                assert(NULL!=meta->node);
                const node_type *node = meta->node;
                //______________________________________________________________
                //
                // two stages comparison
                //______________________________________________________________
                if( (hkey==meta->hkey) && (key==node->key()) )
                {
                    return load.move_to_front(meta)->node; //!< moved to front of slot
                }
            }
            return NULL;
        }

        //______________________________________________________________________
        //
        //! generic insert function
        //______________________________________________________________________
        inline bool insert(const size_t    hkey,
                           const_key_type &key,
                           NODE           *node,
                           pool_type      &pool,
                           quit_proc       quit)
        {
            assert(NULL!=node);
            assert(NULL!=quit);
            //__________________________________________________________________
            //
            // find slot
            //__________________________________________________________________
            slot_type &load = slot[hkey&smask];

            //__________________________________________________________________
            //
            // scan slot
            //__________________________________________________________________
            for(const meta_node *meta=load.head;meta;meta=meta->next)
            {
                assert(NULL!=meta->node);
                //______________________________________________________________
                //
                // two stages comparison
                //______________________________________________________________
                if( (hkey==meta->hkey) && (key==meta->node->key()) )
                {
                    pool.store(quit(node)); //!< already exists
                    return false;
                }
            }

            //__________________________________________________________________
            //
            // new entry
            //__________________________________________________________________
            try {
                meta_node  *meta   = load.push_front( repo.size ? repo.query() : new meta_node() );
                meta->node         = node;
                coerce(meta->hkey) = hkey;
                data.push_back(node);
            }
            catch(...)
            {
                pool.store(quit(node));
                throw;
            }
            return true;
        }

        void free_with(pool_type &pool,
                       quit_proc  quit) throw()
        {
            //__________________________________________________________________
            //
            // remove data
            //__________________________________________________________________
            assert(NULL!=quit);
            while(data.size) pool.store( quit(data.pop_back()) );

            // cleanup slots
            for(size_t i=0;i<slots;++i)
            {
                slot_type &load = slot[i];
                while(load.size) repo.store( load.pop_back()->free() );
            }
        }



    private:
        YACK_DISABLE_COPY_AND_ASSIGN(hash_table);
        list_type  data;  //!< user's data
        slot_type *slot;  //!< first slot
        size_t     slots; //!< a power of two, 1=>use base
        size_t     smask; //!< slots-1
        meta_pool  repo;  //!< meta node repository
        slot_type  base;  //!< initial slot



    };

}

#endif

