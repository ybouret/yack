//! \file

#ifndef YACK_DATA_HASH_TABLE_INCLUDED
#define YACK_DATA_HASH_TABLE_INCLUDED 1

#include "yack/data/list.hpp"
#include "yack/data/hash/node.hpp"
#include "yack/data/pool/cxx.hpp"
#include "yack/type/out-of-reach.hpp"
#include "yack/arith/base2.hpp"
#include "yack/arith/ilog2.hpp"
#include <iostream>

namespace yack
{
    struct hash_table_
    {
        static void *acquire(const size_t page_exp2);
        static void  release(void *the_addr, const size_t page_exp2) throw();
        static void  check(const size_t user_exp2, const size_t maxi_exp2);
    };

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
            // must have no data!
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


        //______________________________________________________________________
        //
        //! free all nodes, keep resources
        //______________________________________________________________________
        inline void free_with(pool_type &pool,
                              quit_proc  quit) throw()
        {
            //__________________________________________________________________
            //
            // remove data
            //__________________________________________________________________
            assert(NULL!=quit);
            while(data.size) pool.store( quit(data.pop_back()) );

            //__________________________________________________________________
            //
            // cleanup slots
            //__________________________________________________________________
            for(size_t i=0;i<slots;++i)
            {
                slot_type &load = slot[i];
                while(load.size) repo.store( load.pop_back()->free() );
            }
        }

#if 0
        inline void release_with(quit_proc kill) throw()
        {
            //__________________________________________________________________
            //
            // remove data
            //__________________________________________________________________
            assert(NULL!=kill);
            while(data.size)  kill(data.pop_back());

            //__________________________________________________________________
            //
            // cleanup slots
            //__________________________________________________________________
            for(size_t i=0;i<slots;++i)
            {
                slot_type &load = slot[i];
                while(load.size) delete load.pop_back();
            }
            repo.release();
        }
#endif

        inline size_t size() const throw() { return data.size; }

        inline void reload(const size_t new_sexp2) throw()
        {


#if 0
            if(new_sexp2!=sexp2)
            {
                if(0==new_sexp2)
                {
                    assert(slot!=&base);
                    assert(sexp2!=0);
                    hash_table_::release(slot,sexp2+slot_exp2);
                    slot = &base;
                    sexp2 = 0;
                    smask = 0;
                    slots = 1;
                }
                else
                {
                    // acquire new resources
                    hash_table_::check(new_sexp2,maxi_exp2);
                    const size_t page_exp2 = new_sexp2 + slot_exp2;  //!< page_exp2
                    const size_t new_slots = size_t(1) << new_sexp2; //!< new slots
                    const size_t new_smask = new_slots-1;            //!< new smask
                    slot_type   *new_slot  = static_cast<slot_type *>( hash_table_::acquire(page_exp2) );

                    // format new resources
                    for(size_t i=0;i<new_slots;++i) new (new_slot+i) slot_type();

                    // move meta nodes
                    for(size_t i=0;i<slots;++i)
                    {
                        slot_type &load = slot[i];
                        while(load.size)
                        {
                            meta_node *meta = load.pop_back();
                            new_slot[meta->hkey&new_smask].push_front(meta);
                        }
                    }

                    // change resources
                    if(sexp2!=0)
                    {
                        hash_table_::release(slot,sexp2+slot_exp2);
                    }
                    slot  = new_slot;
                    sexp2 = new_sexp2;
                    smask = new_smask;
                    slots = new_slots;

                }
            }
#endif
        }


    private:
        YACK_DISABLE_COPY_AND_ASSIGN(hash_table);
        list_type  data;  //!< user's data
        slot_type *slot;  //!< first slot
        size_t     slots; //!< a power of two, 1=>use base
        size_t     sexp2; //!< slots = 1<< sexp2
        size_t     smask; //!< slots-1
        meta_pool  repo;  //!< meta node repository
        slot_type  base;  //!< initial slot



    };

}

#endif

