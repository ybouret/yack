//! \file

#ifndef YACK_DATA_HASH_TABLE_INCLUDED
#define YACK_DATA_HASH_TABLE_INCLUDED 1

#include "yack/data/list.hpp"
#include "yack/data/hash/node.hpp"
#include "yack/data/pool/cxx.hpp"
#include "yack/type/out-of-reach.hpp"
#include "yack/arith/ilog2.hpp"
#include "yack/arith/base2.hpp"
#include "yack/type/destruct.hpp"
#include <iostream>

namespace yack
{
    namespace kernel
    {
        //______________________________________________________________________
        //
        //
        //! internal memory for hash_table
        //
        //______________________________________________________________________
        struct hash_table_
        {
            static void   *acquire(const size_t page_exp2);                    //!< 2^page_exp2 bytes
            static void    release(void *, const size_t page_exp2) throw();    //!< release 2^page_exp2 bytes
            static size_t  check(const size_t usr_exp2,const size_t max_exp2); //!< check limit, raise error on overflow
        };

        //______________________________________________________________________
        //
        //
        //! basic hash_table
        //
        //______________________________________________________________________
        template <typename KEY, typename NODE>
        class hash_table
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            YACK_DECL_ARGS_(KEY,key_type);                    //!< aliases
            typedef NODE                   node_type;         //!< alias
            typedef list_of<NODE>          list_type;         //!< live nodes
            typedef pool_of<NODE>          pool_type;         //!< for user to get back node
            typedef hash_node<KEY,NODE>    meta_node;         //!< for slots
            typedef list_of<meta_node>     slot_type;         //!< slot type
            typedef cxx_pool_of<meta_node> meta_pool;         //!< pool of meta nodes
            typedef NODE *               (*quit_proc)(NODE*); //!< clean NODE on error

            //__________________________________________________________________
            //
            //! internal linear table of slots
            //__________________________________________________________________
            class xtable
            {
            public:
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________
                static const size_t shift    = ilog2_of<slot_type>::value;     //!< sizeof(slot_type) is a power of two
                static const size_t max_exp2 = base2<size_t>::max_shift-shift; //!< to check overflow

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________

                //! no-throw construct from internal slot
                inline explicit xtable(slot_type &mine) throw() :
                exp2(0), slot(&mine), size(1), mask(0) { }

                //! acquire with usr_exp2<=max_exp2
                explicit xtable(size_t usr_exp2) :
                exp2( hash_table_::check(usr_exp2,max_exp2)  ),
                slot( static_cast<slot_type*>(  hash_table_::acquire(usr_exp2+shift) ) ),
                size( size_t(1)<<exp2 ),
                mask( size-1 )
                {
                    for(size_t i=0;i<size;++i) new (slot+i) slot_type();
                }

                //! destruct, MUST be empty
                inline ~xtable() throw()
                {
                    if(exp2>0)
                    {
                        for(size_t i=0;i<size;++i) destruct(slot+i);
                        hash_table_::release(slot,exp2+shift);
                    }
                    slot        =0;
                    coerce(size)=0;
                    coerce(mask)=0;
                    coerce(exp2)=0;
                }

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________

                //! no-throw swap
                inline void swap_with(xtable &other) throw()
                {
                    coerce_cswap(exp2,other.exp2);
                    coerce_cswap(slot,other.slot);
                    coerce_cswap(size,other.size);
                    coerce_cswap(mask,other.mask);
                }

                //! steal and dispatch nodes
                inline void steal(xtable &other) throw()
                {
                    const size_t n=other.size;
                    for(size_t i=0;i<n;++i)
                    {
                        slot_type &source = other.slot[i];
                        while(source.size)
                        {
                            meta_node *meta = source.pop_back(); assert(NULL!=meta->node);
                            slot[meta->hkey&mask].push_front(meta);
                        }
                    }

                }

                //! slot access
                inline slot_type &operator[](const size_t hkey) throw()
                {
                    assert(slot);
                    return slot[hkey&mask];
                }


                //! slot access, const
                inline const slot_type &operator[](const size_t hkey) const throw()
                {
                    assert(slot);
                    return slot[hkey&mask];
                }

                //! return all freed meta_nodes to repository
                inline void back_to(meta_pool &repo) throw()
                {
                    for(size_t i=0;i<size;++i) store(repo,slot[i]);
                }

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const size_t exp2; //!< for size

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(xtable);
                slot_type   *slot; //!< first slot
            public:
                const size_t size; //!< size=2^exp2
                const size_t mask; //!< mask=size-1

            private:
                static inline void store(meta_pool &repo, slot_type &load) throw()
                {
                    while(load.size) repo.store( load.pop_back()->freed() );
                }
            };


            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! setup
            inline explicit hash_table() throw() :
            data(), base(), xtab(base), repo()
            {
            }

            //! cleanup
            inline virtual ~hash_table() throw()
            {
                // must have no data!
            }



            //! generic search function, const
            inline const node_type *search(const size_t    hkey,
                                           const_key_type &key) const throw()
            {

                //______________________________________________________________
                //
                // find slot
                //______________________________________________________________
                const slot_type     &load = xtab[hkey];

                //______________________________________________________________
                //
                // scan slot
                //______________________________________________________________
                for(const meta_node *meta = load.head;meta;meta=meta->next)
                {
                    assert(NULL!=meta->node);
                    const node_type *node = meta->node;
                    //__________________________________________________________
                    //
                    // two stages comparison
                    //__________________________________________________________
                    if( (hkey==meta->hkey) && (key==node->key()) )
                    {
                        return node;
                    }
                }
                return NULL;
            }

            //! generic insert function
            inline bool insert(const size_t    hkey,
                               const_key_type &key,
                               NODE           *node,
                               pool_type      &pool,
                               quit_proc       quit)
            {
                assert(NULL!=node);
                assert(NULL!=quit);
                //______________________________________________________________
                //
                // find slot
                //______________________________________________________________
                slot_type &load = xtab[hkey];

                //______________________________________________________________
                //
                // scan slot
                //______________________________________________________________
                for(const meta_node *meta=load.head;meta;meta=meta->next)
                {
                    assert(NULL!=meta->node);
                    //__________________________________________________________
                    //
                    // two stages comparison
                    //__________________________________________________________
                    if( (hkey==meta->hkey) && (key==meta->node->key()) )
                    {
                        pool.store(quit(node)); //!< already exists
                        return false;
                    }
                }

                //______________________________________________________________
                //
                // new entry
                //______________________________________________________________
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

            //! generic remove algorithm
            inline bool remove(const size_t    hkey,
                               const_key_type &key,
                               pool_type      &pool,
                               quit_proc       quit) throw()
            {
                assert(NULL!=quit);
                slot_type     &load = xtab[hkey]; // scan this slot
                for(meta_node *meta=load.head;meta;meta=meta->next)
                {
                    assert(meta->node);
                    if( (hkey==meta->hkey) && (key==meta->node->key()) )
                    {
                        pool.store( quit(data.pop(meta->node)) );
                        repo.store( load.pop(meta)->freed() );
                        return true;
                    }
                }
                return false;
            }



            //! free all nodes, keep resources
            inline void free_with(pool_type &pool,
                                  quit_proc  quit) throw()
            {
                //______________________________________________________________
                //
                // remove data
                //______________________________________________________________
                assert(NULL!=quit);
                while(data.size) pool.store( quit(data.pop_back()) );

                //______________________________________________________________
                //
                // cleanup slots
                //______________________________________________________________
                xtab.back_to(repo);
            }


            //! access data
            inline const list_of<NODE> & operator*() const throw() { return data; }

            //! get current slots
            inline size_t slots()        const throw() { return xtab.size; }

            //! get average load
            inline size_t average_load() const throw() { return data.size/xtab.size; }

            //! change internal table size to 2^new_exp2
            inline void reload(const size_t new_exp2)
            {
                if(new_exp2!=xtab.exp2)
                {
                    if(new_exp2<=0)
                        exchange(base);        // no-throw
                    else
                        exchange(new_exp2);    // may throw
                }
            }

            //! compute exp2 for xtable to preserve load_factor
            size_t exp2_for(const size_t load_factor) const throw()
            {
                const size_t total = data.size;
                size_t       shift = 0;
                size_t       count = 1;

                while(total/count>load_factor)
                {
                    count <<= 1;
                    ++shift;
                }
                return shift;
            }

            //! try to adjust table size to match load_factor
            void adjust_for(const size_t load_factor)
            {
                const size_t total = data.size;
                size_t       count = xtab.size;

                if(total/count>load_factor)
                {
                    size_t shift = xtab.exp2;
                    while(total/count>load_factor)
                    {
                        count <<= 1;
                        ++shift;
                    }
                    reload(shift);
                }

            }

            //! reserve empty meta nodes
            void reserve(size_t n)
            {
                while(n-- > 0) repo.store( new meta_node() );
            }

            //! delete cache meta nodes
            inline void trim() throw()
            {
                repo.release();
            }

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(hash_table);
            list_type  data;  //!< user's data
            slot_type  base;  //!< initial slot
            xtable     xtab;  //!< table of slots
            meta_pool  repo;  //!< meta node repository

            template <typename ARGS> inline
            void exchange(ARGS &args)
            {
                xtable tmp(args);
                tmp.steal(xtab);
                xtab.swap_with(tmp);
            }

        };

    }

}

#endif

