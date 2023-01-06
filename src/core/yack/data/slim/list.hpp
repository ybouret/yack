//! \file

#ifndef YACK_DATA_SLIM_LIST_INCLUDED
#define YACK_DATA_SLIM_LIST_INCLUDED 1

#include "yack/data/slim/node.hpp"
#include "yack/data/slim/root.hpp"
#include "yack/data/list/sort.hpp"

namespace yack
{

    //__________________________________________________________________________
    //
    //
    //! list of alive slim_node<T>, can be mixed with cxx_list_of< slim_node<T> >
    //
    //__________________________________________________________________________
    template <typename T>
    class slim_list : public  slim_root< slim_node<T> > 
    {
    public:
        //______________________________________________________________________
        //
        // types
        //______________________________________________________________________
        YACK_DECL_ARGS_(T,type);                   //!< aliases
        typedef slim_node<T>         node_type;  //!< alias
        typedef slim_root<node_type> list_type;  //!< alias

        using list_type::push_back;
        using list_type::pop_back;
        using list_type::push_front;
        using list_type::pop_front;
        using list_type::clear;

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline explicit slim_list() throw() : list_type() {}   //!< setup empty
        inline virtual ~slim_list() throw()  {  clear(); }     //!< cleanup
        inline slim_list(const slim_list &_) : list_type(_) {} //!< copy

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________


        //! push back new value, return node
        template <typename U> inline
        node_type *annex(const U &u) { return push_back( new node_type(u,transmogrify) ); }


        //! push front new value, return node
        template <typename U> inline
        node_type *shove(const U &u) { return push_front( new node_type(u,transmogrify) ); }

        //! push_back wrapper
        template <typename U>
        slim_list & operator<<(const U &args) {
            (void) push_back( new node_type(args,transmogrify) );
            return *this;
        }

        //! push_front wrapper
        template <typename U>
        slim_list & operator>>(const U &args) {
            (void) push_front( new node_type(args,transmogrify) );
            return *this;
        }

        
        //! insert new node after mine, return node
        template <typename U>
        node_type * after(node_type *mine, const U &args)
        {
            return this->insert_after(mine, new node_type(args,transmogrify) );
        }


        //! insert new node before mine, return node
        template <typename U>
        node_type *ahead(node_type *mine, const U &args)
        {
           return this->insert_before(mine, new node_type(args,transmogrify) );
        }

        

        //! cut head and return its value
        inline type pop_head() {
            assert(this->head);
            type res = **(this->head); this->cut_head(); return res;
        }

        //! cut tail and return its value
        inline type pop_tail() {
            assert(this->tail);
            type res = **(this->tail); this->cut_tail(); return res;
        }


        //! slow access
        inline type & operator[](const size_t indx) throw() {
            assert(indx>0); assert(indx<=this->size);
            return **(this->get(indx));
        }

        //! slow const access
        inline const_type & operator[](const size_t indx) const throw() {
            assert(indx>0); assert(indx<=this->size);
            return **(this->get(indx));
        }


        //! sort function
        template <typename FUNC> inline
        void sort_with( FUNC &func )
        {
            kernel::primary_sort_callback<node_type,FUNC> proc = { func };
            merge_list_of<node_type>::sort(*this,proc);
        }

        
    private:
        YACK_DISABLE_ASSIGN(slim_list);
        virtual void kill_one(node_type *node) throw() { assert(node); delete node; }
        virtual void kill_all()                throw() { while(this->size) delete pop_back(); }
    };

}

#endif

