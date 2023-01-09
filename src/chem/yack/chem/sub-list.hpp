
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

            //! setup with host and positive index
            inline explicit sub_node(const T &user, const size_t i) throw() :
            object(),
            indexed(i),
            host(user),
            next(0),
            prev(0)
            {
            }

            //! cleanup
            inline virtual ~sub_node() throw() {}

            //! copy
            inline sub_node(const sub_node &other) throw() :
            object(), indexed(other), host(other.host), next(0), prev(0) {}

            // methods

            inline friend std::ostream & operator<<(std::ostream &os, const sub_node &self)
            {
                os << self.host << "@" << *self;
                return os;
            }

            // members
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
                const size_t i = L.tail ? **(L.tail)+1 : 1;
                L.push_back( new node_type(obj,i) );
                return *this;
            }

            template <typename TARGET, typename SOURCE> inline
            void primary_transfer(TARGET &target, SOURCE &source)
            {
                for(const node_type *node=L.head;node;node=node->next)
                {
                    const size_t   j = *(node->host);
                    target[j] = source[j];

                }
            }

            template <typename TARGET, typename SOURCE> inline
            void replica_transfer(TARGET &target, SOURCE &source)
            {
                for(const node_type *node=L.head;node;node=node->next)
                {
                    const size_t   j = ***node;
                    target[j] = source[j];
                }
            }


        private:
            cxx_list_of<node_type> L;
        };

    }

}

#endif
