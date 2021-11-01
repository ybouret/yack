//! \file

#ifndef YACK_DATA_POOL_SORT_INCLUDED
#define YACK_DATA_POOL_SORT_INCLUDED 1

#include "yack/data/pool.hpp"
#include "yack/comparison.hpp"


namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! assuming a singly linked node
    //
    //__________________________________________________________________________
    template <typename NODE>
    class merge_pool_of
    {
    public:
        
        //______________________________________________________________________
        //
        //! sort pool_of and derived classes
        //______________________________________________________________________
        template <typename POOL, typename COMPARE_NODES> static inline
        void sort(POOL &source, COMPARE_NODES &proc)
        {
            switch(source.size)
            {
                case 0: return;
                case 1: return;
                default:
                {
                    assert(source.size>1);
                    sub_pool L;
                    sub_pool R;
                    const size_t        mid = source.size >> 1;
                    for( size_t i=source.size-mid;i>0;--i) L.store( source.query() );
                    for( size_t i=mid;i>0;--i)             R.store( source.query() );
                    assert(0==source.size);
                    
                    merge_pool_of<NODE>::template sort<sub_pool>(L,proc);
                    merge_pool_of<NODE>::template sort<sub_pool>(R,proc);
                    merge_pool_of<NODE>::template fusion<POOL>(source,L,R,proc);
                }
            }
        }
        
        //! helper to sort by decreasing node address
        template <typename POOL> static inline
        void sort_by_increasing_address(POOL &source)
        {
            sort(source,comparison::increasing<const NODE*>);
        }
        
        //! helper to sort by decreasing node address
        template <typename POOL> static inline
        void sort_by_decreasing_address(POOL &source)
        {
            sort(source,comparison::decreasing<const NODE*>);
        }
        
    private:
        typedef  pool_of<NODE> sub_pool;
        
        //! merge two sorted list
        template <typename POOL,typename COMPARE_NODES>
        static inline
        void fusion( POOL &target, sub_pool &L, sub_pool &R, COMPARE_NODES &proc)
        {
            //! while we can compare two nodes
            assert(0==target.size);
            while( (L.size>0) && (R.size>0) )
            {
                if( proc(L.head,R.head) < 0 )
                {
                    target.store( L.query() );
                }
                else
                {
                    target.store( R.query() );
                }
            }
            
            //! append remaining pools
            assert(0==L.size||0==R.size);
            while( L.size > 0 ) target.store( L.query() );
            while( R.size > 0 ) target.store( R.query() );
            target.reverse();
        }
        
    };
}

#endif

