//! \file

#ifndef YACK_DATA_LIST_SORT_INCLUDED
#define YACK_DATA_LIST_SORT_INCLUDED 1

#include "yack/data/list.hpp"
#include "yack/comparison.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! merge sort for lists
    //
    //__________________________________________________________________________
    template <typename NODE>
    struct merge_list_of
    {
        //______________________________________________________________________
        //
        //! for list_of and derived classes
        //__________________________________________________________________________
        template <typename LIST, typename COMPARE_NODES> static inline
        void sort(LIST &source, COMPARE_NODES &compare_nodes)
        {
            switch(source.size)
            {
                case 0: return;
                case 1: return;
                default:
                {
                    assert(source.size>1);
                    sub_list L;
                    sub_list R;
                    const size_t        mid = source.size >> 1;
                    for( size_t i=source.size-mid;i>0;--i) L.push_front( source.pop_back() );
                    for( size_t i=mid;i>0;--i)             R.push_front( source.pop_back() );
                    assert(0==source.size);
                    
                    merge_list_of<NODE>::template sort<sub_list>(L,compare_nodes);
                    merge_list_of<NODE>::template sort<sub_list>(R,compare_nodes);
                    merge_list_of<NODE>::template fusion<LIST>(source,L,R,compare_nodes);
                }
            }
        }
        
        //! helper to sort by increasing node address
        template <typename LIST> static inline
        void sort_by_increasing_address(LIST &source)
        {
            sort(source,comparison::increasing<const NODE*>);
        }
        
        //! helper to sort by decreasing node address
        template <typename LIST> static inline
        void sort_by_decreasing_address(LIST &source)
        {
            sort(source,comparison::decreasing<const NODE*>);
        }
        
        
    private:
        typedef list_of<NODE> sub_list; //!< internal type

        //! merge two sorted list
        template <typename LIST, typename COMPARE_NODES>
        static inline
        void fusion(LIST &target, sub_list &L, sub_list &R, COMPARE_NODES &compare_nodes )
        {
            //! while we can compare two nodes
            assert(0==target.size);
            while( (L.size>0) && (R.size>0) )
            {
                if( compare_nodes(L.head,R.head) < 0 )
                {
                    target.push_back( L.pop_front() );
                }
                else
                {
                    target.push_back( R.pop_front() );
                }
            }
            
            //! append remaining lists
            assert(0==L.size||0==R.size);
            while( L.size > 0 ) target.push_back( L.pop_front() );
            while( R.size > 0 ) target.push_back( R.pop_front() );
        }
        
        
        
    };
    
}

#endif

