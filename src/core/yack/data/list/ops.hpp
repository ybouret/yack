
//! \file

#ifndef YACK_DATA_LIST_OPS_INCLUDED
#define YACK_DATA_LIST_OPS_INCLUDED 1

#include "yack/data/list.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! operation on list
    //
    //__________________________________________________________________________
    struct list_ops
    {
        
        //______________________________________________________________________
        //
        //! test node-wise equality
        //______________________________________________________________________
        template <typename LHS, typename RHS> static inline
        bool are_equal(const list_of<LHS> &lhs, const list_of<RHS> &rhs)
        {
            size_t n = lhs.size;
            if(n != rhs.size)
            {
                return false;
            }
            else
            {
                const LHS *l = lhs.head;
                const RHS *r = rhs.head;
                while(n-- > 0)
                {
                    if( **l != **r ) return false;
                    l=l->next;
                    r=r->next;
                }
                return true;
            }
        }

        //______________________________________________________________________
        //
        //! search an ORDERED list
        /**
         \param l list of NODEs
         \param x value to match
         \param p matching or previous node
         */
        //______________________________________________________________________
        template <typename NODE, typename T> static inline
        bool search(const list_of<NODE> &l,
                    const T             &x,
                    NODE               *&p)
        {
            //------------------------------------------------------------------
            //
            // initialize: no previous => push_front
            //
            //------------------------------------------------------------------
            p = 0;
            switch(l.size)
            {
                case 0: return false; // would push first item
                case 1:
                    switch( __sign::of(x,**l.head) )
                    {
                        case negative:             return false; // before head
                        case __zero__: p = l.head; return true;  // found @head
                        case positive: p = l.head; return false; // after head
                    }
                default:
                    break;
            }
            //------------------------------------------------------------------
            //
            // generic case
            //
            //------------------------------------------------------------------
            assert(l.size>=2);

            //------------------------------------------------------------------
            // check head
            //------------------------------------------------------------------
            NODE *lo = l.head;
            switch( __sign::of(x,**lo) )
            {
                case negative:         return false; // before head
                case __zero__: p = lo; return true;  // found @head
                case positive: break;
            }

            //------------------------------------------------------------------
            // check tail now that x > **head
            //------------------------------------------------------------------
            NODE *up = l.tail;
            switch( __sign::of(x,**up) )
            {
                case positive: p = up; return false; // after tail
                case __zero__: p = up; return true;  // found @tail
                case negative: break;
            }

            //------------------------------------------------------------------
            // look up now (x>**head) and (x<**tail)
            //------------------------------------------------------------------
        TRY_NEXT:
            NODE *nx = lo->next; assert(nx);
            if(nx==up) {
                // found the interval
                p = lo;
                return false;
            }
            else
            {
                // check status
                switch( __sign::of(x,**nx) )
                {
                    case __zero__: p=nx; return true;    // found @nx
                    case negative: p=lo; return false;   // between lo and nx
                    case positive: lo=nx; break;
                }

                goto TRY_NEXT; // forward
            }
            
        }

        //______________________________________________________________________
        //
        //! check if is strictly increasing
        //______________________________________________________________________
        template <typename NODE> static inline
        bool is_increasing(const list_of<NODE> &L) {
            const NODE *curr = L.head;
            while(curr&&curr->next)
            {
                const NODE *next = curr->next;
                if( **next <= **curr ) return false;
                curr = next;
            }
            return true;
        }



    };

}

#endif
