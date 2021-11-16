//! \file

#ifndef YACK_DATA_HASH_TABLE_INCLUDED
#define YACK_DATA_HASH_TABLE_INCLUDED 1

#include "yack/data/list.hpp"
#include "yack/data/hash/node.hpp"
#include "yack/data/pool/cxx.hpp"


namespace yack
{

    //! basic hash_table
    template <typename KEY, typename NODE>
    class hash_table
    {
    public:
        YACK_DECL_ARGS(KEY,key_type);
        typedef NODE                   node_type; //!< alias
        typedef cxx_list_of<NODE>      list_type; //!< live nodes
        typedef pool_of<NODE>          pool_type; //!< for user to get back node
        typedef hash_node<KEY,NODE>    meta_node; //!< for slots
        typedef list_of<meta_node>     slot_type; //!< slot type
        typedef cxx_pool_of<meta_node> meta_pool; //!< pool of meta nodes


        inline explicit hash_table() throw() :
        data(), slot(0), slots(1), smask(0), base()
        {
        }

        inline virtual ~hash_table() throw()
        {

        }

        inline const node_type *search(const size_t    hkey,
                                       const_key_type &key) throw()
        {
            assert(slot);
            const slot_type     &load = slot[hkey&smask];
            for(const meta_node *meta = load.head;meta;meta=meta->next) {
                const node_type *node = meta->node
                assert(NULL!=meta->node);
                if( (hkey==meta->hkey) && (key==meta->node->key()) )
                    return node;
            }
            return NULL;
        }

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(hash_table);
        list_type  data;  //!< user's data
        slot_type *slot;  //!< first slot
        size_t     slots; //!< a power of two, 1=>use base
        size_t     smask; //!< slots-1
        slot_type  base;  //!< initial slot

    };

}

#endif

