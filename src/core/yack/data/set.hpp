//! \file

#ifndef YACK_DATA_SET_INCLUDED
#define YACK_DATA_SET_INCLUDED 1

#include "yack/data/dinky/coop-list.hpp"
#include "yack/data/list/ops.hpp"

namespace yack
{

    //__________________________________________________________________________
    //
    //
    //! data set of unique value, internally ordered
    //
    //__________________________________________________________________________
    template <typename T>
    class data_set
    {
    public:
        //______________________________________________________________________
        //
        // types
        //______________________________________________________________________
        YACK_DECL_ARGS(T,type);                           //!< aliases
        typedef coop_list<T>                   list_type; //!< internal list
        typedef typename list_type::node_type  node_type; //!< alias
        typedef typename list_type::zpool_type bank_type; //!< alias
        typedef typename list_type::proxy_type fund_type; //!< alias

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________

        //! setup with cache
        inline explicit data_set(const fund_type &fund) noexcept : dlist(fund) {}

        //! cleanup
        inline virtual ~data_set() noexcept {}

        //! copy
        inline data_set(const data_set &_) : dlist(_.dlist) {}

        //! assign by copy/swap
        inline data_set & operator=(const data_set &other) {
            data_set temp(other);
            dlist.swap_with(other.dlist);
            return *this;
        }

        //______________________________________________________________________
        //
        // access methods
        //______________________________________________________________________
        const list_type * operator->() const noexcept { return &dlist; } //!< access, const
        const list_type & operator*()  const noexcept { return  dlist; } //!< access, const

        //______________________________________________________________________
        //
        // methods for one item
        //______________________________________________________________________

        //! try to insert a new value
        inline bool insert(param_type args)
        {
            node_type *p = 0;
            if( search(args,p) )
            {
                assert(0!=p);
                return false; // already in
            }
            else
            {
                if(0!=p)
                {
                    dlist.after(p,args);
                }
                else
                {
                    dlist.shove(args);
                }
                return true;
            }
        }



        //! check if a value is contained
        inline bool contains(param_type args) const   {
            node_type *p = NULL;
            return search(args,p);
        }

        //! try to remove a value
        inline bool remove(param_type args) {
            node_type *p = NULL;
            if( search(args,p) )
            {
                assert(NULL!=p); dlist.cut(p); return true;
            }
            else
                return false;
        }

        inline void       ensure(param_type args)     { (void) insert(args); }         //!< ensure a value is in the set
        inline void       dismiss(param_type args)    { (void) remove(args); }         //!< ensure a value is not present
        inline data_set & operator+=(param_type args) { ensure(args); return *this; }  //!< ensure(args)
        inline data_set & operator-=(param_type args) { dismiss(args); return *this; } //!< dismiss(args)
        inline void       pop_upper() noexcept         { dlist.cut_tail(); }            //!< remove upper value
        inline void       pop_lower() noexcept         { dlist.cut_head(); }            //!< remove lower value
        inline type       pull_upper()                { return dlist.pull_tail(); }    //!< pull upper value
        inline type       pull_lower()                { return dlist.pull_head(); }    //!< pull lower value

        //______________________________________________________________________
        //
        // methods for lists
        //______________________________________________________________________

        //! all other data must be in this set
        inline bool includes(const data_set &other) const
        {
            //------------------------------------------------------------------
            // get rid of trivial cases
            //------------------------------------------------------------------
            if(this == &other)              return true;  // same set
            if(dlist.size<other.dlist.size) return false; // this is too small

            //------------------------------------------------------------------
            // initialize search
            //------------------------------------------------------------------
            const node_type *node = other->head;
            node_type       *mine = NULL;
            if(!search(**node,mine)) return false;
            assert(NULL!=mine);

            //------------------------------------------------------------------
            // look of for other nodes
            //------------------------------------------------------------------
            for(node=node->next;node;node=node->next)
            {
            NEXT_MINE:
                mine=mine->next;
                if(!mine) return false;
                if(**mine!=**node) goto NEXT_MINE;
            }
            return true;
        }


        //! no other data allowed in this set
        inline bool excludes(const data_set &other) const
        {
            for(const node_type *node=other->head;node;node=node->next)
            {
                node_type *mine=NULL;
                if(search(**node,mine)) return false;
            }
            return true;
        }


        //! merge all foreign data
        inline void merge(const data_set &other)
        {
            if(this!=&other) {
                for(const node_type *node=other->head;node;node=node->next)
                    ensure(**node);
            }
        }

        //! purge all common data
        inline void purge(const data_set &other)
        {
            if(this!=&other) {
                for(const node_type *node=other->head;node;node=node->next)
                    dismiss(**node);
            }
            else
                free();
        }


        inline void       free() noexcept                    { dlist.clear(); }              //!< free content
        inline data_set & operator+=(const data_set &other) { merge(other); return *this; } //!< merge other
        inline data_set & operator-=(const data_set &other) { purge(other); return *this; } //!< purge other


        //! test equality
        inline friend bool operator==(const data_set &lhs, const data_set &rhs) {
            return list_ops::are_equal(lhs.dlist,rhs.dlist);
        }


    private:
        coop_list<T> dlist;

        inline bool search(const_type &value,
                           node_type * &p) const {
            return list_ops::search(dlist,value,p);
        }
    };

}


#endif
