
//! \file

#ifndef YACK_CHEMICAL_SUB_NODE_INCLUDED
#define YACK_CHEMICAL_SUB_NODE_INCLUDED 1

#include "yack/chem/indexed.hpp"
#include "yack/object.hpp"
#include "yack/data/list/cxx.hpp"

namespace yack
{
    namespace chemical
    {

        template <typename T>
        class sub_node : public object, public indexed
        {
        public:
            inline explicit sub_node(const T &user, const size_t i) throw() :
            object(),
            indexed(i),
            host(user),
            next(0),
            prev(0)
            {
            }

            inline virtual ~sub_node() throw() {}

            inline sub_node(const sub_node &other) throw() :
            object(), indexed(other), host(other.host), next(0), prev(0) {}

            const T  &host;
            sub_node *next;
            sub_node *prev;

        private:
            YACK_DISABLE_ASSIGN(sub_node);
        };

        template <typename T>
        class sub_list
        {
        public:
            typedef sub_node<T>        node_type;
            typedef list_of<node_type> list_type;

            inline explicit sub_list() throw() : L() {}
            inline virtual ~sub_list() throw()       {}

            const list_type * operator->() const throw() { return &L; }
            const list_type & operator*()  const throw() { return  L; }

            inline sub_list & operator<<(const T &obj) {
                const size_t i = L.head ? **(L.head) : 1;
                L.push_back( new node_type(obj,i) );
                return *this;
            }

            template <typename TARGET, typename SOURCE> inline
            void transfer(TARGET &target, SOURCE &source)
            {
                for(const node_type *node=L.head;node;node=node->next)
                {
                    //const size_t i = ***node;

                }
            }


        private:
            cxx_list_of<node_type> L;
        };

    }

}

#endif
