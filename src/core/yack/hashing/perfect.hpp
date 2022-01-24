//! \file

#ifndef YACK_HASHING_PERFECT_INCLUDED
#define YACK_HASHING_PERFECT_INCLUDED 1

#include "yack/object.hpp"
#include "yack/data/list/cxx.hpp"
namespace yack
{

    namespace hashing
    {

        
        class perfect
        {
        public:
            class                          node_type;
            typedef cxx_list_of<node_type> list_type;

            class node_type : public object
            {
            public:
                virtual ~node_type() throw();
                explicit node_type(const int) throw();
                
                node_type *next;
                node_type *prev;
                size_t     freq;
                int        code;

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(node_type);
            };

            explicit perfect();
            virtual ~perfect() throw();

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(perfect);
            node_type *root;
        };

    }

}

#endif

