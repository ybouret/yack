
//! \file

#ifndef YACK_DATA_COM_LIST_INCLUDED
#define YACK_DATA_COM_LIST_INCLUDED 1


#include "yack/data/list/joint.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! node for common pool/list
    //
    //__________________________________________________________________________
    template <typename T>
    class com_node
    {
    public:
        //______________________________________________________________________
        //
        // types
        //______________________________________________________________________
        YACK_DECL_ARGS(T,type); //!< aliases

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline com_node() : next(0), prev(0), data() {}                              //!< setup empty
        inline com_node(const com_node &node) : next(0), prev(0), data(node.data) {} //!< copy node data
        inline com_node(param_type      args) : next(0), prev(0), data(args)      {} //!< copy user data

        //______________________________________________________________________
        //
        // method
        //______________________________________________________________________
        inline type       & operator*()       throw() { return              data; } //!< access
        inline const_type & operator*() const throw() { return (const_type&)data; } //!< access

        //! display data
        inline friend std::ostream & operator<<(std::ostream   &os,
                                                const com_node &self)
        {
            os << self.data;
            return os;
        }

        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
        com_node *next; //!< for list/pool
        com_node *prev; //!< for list

    private:
        YACK_DISABLE_ASSIGN(com_node);
        type data;
    };


    //__________________________________________________________________________
    //
    //
    //! derived class for joint_pool of com_node
    //
    //__________________________________________________________________________
    template <typename T>
    class com_pool : public joint_pool< com_node<T> >
    {
    public:
        //______________________________________________________________________
        //
        // types
        //______________________________________________________________________
        typedef com_node<T>           node_type; //!< alias
        typedef joint_pool<node_type> self_type; //!< alias

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline explicit com_pool() throw() : self_type() {} //!< setup empty
        inline virtual ~com_pool() throw() {}               //!< cleanup
        inline explicit com_pool(const size_t n) :
        self_type() { this->reserve(n); }                   //!< setup with capacity

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(com_pool);
    };


    //__________________________________________________________________________
    //
    //
    //! derived class for joint_list of com_node
    //
    //__________________________________________________________________________
    template <typename T>
    class com_list : public joint_list< com_node<T> >
    {
    public:
        //______________________________________________________________________
        //
        // types
        //______________________________________________________________________
        typedef com_node<T>                  node_type; //!< alias
        typedef joint_list<node_type>        self_type; //!< alias
        typedef joint_pool<node_type>        pool_type; //!< alias
        typedef typename pool_type::pointer  fund_type; //!< alias

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________

        //! setup empty
        inline explicit com_list(const fund_type &sh) throw() : self_type(sh) {}

        //! cleanup
        inline virtual ~com_list() throw() {}




    private:
        YACK_DISABLE_ASSIGN(com_list);
    };

    


}

#endif

