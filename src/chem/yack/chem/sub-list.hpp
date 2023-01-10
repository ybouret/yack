
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
        //______________________________________________________________________
        //
        //
        //! node for sub list: host+(sub)indexed
        /**
         - dedicated for the use in cluster to compress phase space
         - '**this' will return the sub index
         - '*(this->host)' will return the main index
         */
        //
        //______________________________________________________________________
        template <typename T>
        class sub_node : public object, public indexed
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
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

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! specific display
            inline friend std::ostream & operator<<(std::ostream &os, const sub_node &self)
            {
                os << self.host << "@" << *self;
                return os;
            }

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const T  &host; //!< host
            sub_node *next; //!< for list
            sub_node *prev; //!< for list

        private:
            YACK_DISABLE_ASSIGN(sub_node);
        };

        //______________________________________________________________________
        //
        //
        //! sub list of hosted types
        //
        //______________________________________________________________________
        template <typename T>
        class sub_list : public object
        {
        public:
            //__________________________________________________________________
            //
            // types
            //__________________________________________________________________
            typedef sub_node<T>        node_type; //!< alias
            typedef list_of<node_type> list_type; //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            inline explicit sub_list() throw() : L() {}
            inline virtual ~sub_list() throw()       {}

            //__________________________________________________________________
            //
            // access
            //__________________________________________________________________
            inline const list_type * operator->() const throw() { return &L; } //!< access, const
            inline const list_type & operator*()  const throw() { return  L; } //!< access, const

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! automatic growth
            inline sub_list & operator<<(const T &obj) {
                const size_t i = L.tail ? **(L.tail)+1 : 1;
                L.push_back( new node_type(obj,i) );
                return *this;
            }

            //! helper to transfer primary/global arrays
            template <typename TARGET, typename SOURCE> inline
            void primary_transfer(TARGET &target, SOURCE &source)
            {
                for(const node_type *node=L.head;node;node=node->next)
                {
                    const size_t j = *(node->host);
                    target[j] = source[j];

                }
            }

            //! helper to transfer replica/local arrays
            template <typename TARGET, typename SOURCE> inline
            void replica_transfer(TARGET &target, SOURCE &source)
            {
                for(const node_type *node=L.head;node;node=node->next)
                {
                    const size_t j = ***node;
                    target[j] = source[j];
                }
            }


        private:
            cxx_list_of<node_type> L;
        };

    }

}

#endif
