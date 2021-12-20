//! \file

#ifndef YACK_IOS_ASCII_LOADER_INCLUDED
#define YACK_IOS_ASCII_LOADER_INCLUDED 1

#include "yack/singleton.hpp"
#include "yack/data/suffix/tree.hpp"
#include "yack/system/rtti.hpp"

namespace yack
{
    namespace ios
    {
        namespace ascii
        {

            class loader : public singleton<loader>
            {
            public:
                static const char               call_sign[];
                static const at_exit::longevity life_time = 10000;

                typedef void (*builder)(void *,const char *,const size_t,const char *);
                typedef kernel::suffix_tree<builder,uint8_t> tree_type;

                builder & operator[](const rtti &) const;


            private:
                YACK_DISABLE_COPY_AND_ASSIGN(loader);
                explicit loader();
                virtual ~loader() throw();
                friend class singleton<loader>;
                tree_type tree;


            };


        }
    }
}

#endif

