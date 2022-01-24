//! \file

#ifndef YACK_HASHING_PERFECT_INCLUDED
#define YACK_HASHING_PERFECT_INCLUDED 1

#include "yack/object.hpp"
#include "yack/data/list.hpp"
#include "yack/ios/fwd.hpp"
#include "yack/string/fwd.hpp"
#include "yack/memory/buffer/ro.hpp"

namespace yack
{


    namespace hashing
    {

        //______________________________________________________________________
        //
        //
        //! minimal perfect hashing by suffix tree
        //
        //______________________________________________________________________
        class perfect
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit perfect();
            virtual ~perfect() throw();
            explicit perfect(const char * keys[], const size_t size);


            //__________________________________________________________________
            //
            // insertion
            //__________________________________________________________________
            void insert(const void *addr, size_t size); //!< insert a valid key
            void insert(const char *);                  //!< insert text
            void insert(const memory::ro_buffer &);     //!< insert buffer

            //! syntax helper
            template <typename KEY> inline
            perfect & operator<<(const KEY &key) {
                insert(key); return *this;
            }

            //__________________________________________________________________
            //
            // search: negative means error
            //__________________________________________________________________
            int  operator()(const void *addr, size_t size) const throw(); //!< search a valid key
            int  operator()(const char *)                  const throw(); //!< search text
            int  operator()(const memory::ro_buffer&)      const throw(); //!< search buffer

            //__________________________________________________________________
            //
            // graphViz
            //__________________________________________________________________
            void gv(const string &filename) const; //!< internal graphviz
            void gv(const char   *filename) const; //!< internal graphviz

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(perfect);
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            class                       node_type;
            typedef  list_of<node_type> list_type;  

            //__________________________________________________________________
            //
            // internal node
            //__________________________________________________________________
            class node_type : public object
            {
            public:
                virtual ~node_type() throw();
                explicit node_type(node_type    *parent,
                                   const uint8_t encode) throw();

                node_type    *next;
                node_type    *prev;
                node_type    *from;
                size_t        freq;
                list_type     chld;
                int           hash;
                const uint8_t code;

                void gv(ios::ostream&,const bool isRoot=false) const;

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(node_type);
            };

            node_type *root;
            int        hash;
        };

        //! helper for static tables
#define YACK_HASHING_PERFECT(KEYS) KEYS,sizeof(KEYS)/sizeof(KEYS[0])

    }

}

#endif

