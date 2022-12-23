
//! \file

#ifndef YACK_DATA_COM_LIST_INCLUDED
#define YACK_DATA_COM_LIST_INCLUDED 1


#include "yack/data/list/joint.hpp"

namespace yack
{

    template <typename T>
    class com_node
    {
    public:
        YACK_DECL_ARGS(T,type);

        com_node() : next(0), prev(0), data() {}
        com_node(const com_node &node) : next(0), prev(0), data(node.data) {}
        com_node(param_type      args) : next(0), prev(0), data(args)      {}

        type       & operator*()       throw() { return data; }
        const_type & operator*() const throw() { return data; }

        inline friend std::ostream & operator<<(std::ostream   &os,
                                                const com_node &self)
        {
            os << self.data;
            return os;
        }

        com_node *next;
        com_node *prev;

    private:
        YACK_DISABLE_ASSIGN(com_node);
        mutable_type data;
    };

    template <typename T>
    class com_pool : public joint_pool< com_node<T> >
    {
    public:
        typedef com_node<T>           node_type;
        typedef joint_pool<node_type> self_type;

        inline explicit com_pool() throw() : self_type() {}
        inline virtual ~com_pool() throw() {}

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(com_pool);
    };


    template <typename T>
    class com_list : public joint_list< com_node<T> >
    {
    public:
        typedef com_node<T>                 node_type;
        typedef joint_list<node_type>       self_type;
        typedef joint_pool<node_type>       bank_type; //!< alias
        typedef typename bank_type::pointer fund_type; //!< alias

        inline explicit com_list(const fund_type &sh) throw() : self_type(sh) {}
        inline virtual ~com_list() throw() {}




    private:
        YACK_DISABLE_ASSIGN(com_list);
    };

    


}

#endif

