//! \file

#ifndef YACK_IOS_ASCII_CONVERTER_INCLUDED
#define YACK_IOS_ASCII_CONVERTER_INCLUDED 1

#include "yack/singleton.hpp"
#include "yack/data/suffix/tree.hpp"

namespace yack
{

    class rtti;


    namespace ios
    {
        namespace ascii
        {

            class converter : public singleton<converter>
            {
            public:
                static const char               call_sign[];
                static const at_exit::longevity life_time = 10000;

                typedef void (*builder)(void *,const char *,const size_t,const char *);
                typedef kernel::suffix_tree<builder,uint8_t> tree_type;

                builder & operator[](const rtti &) const;


            private:
                YACK_DISABLE_COPY_AND_ASSIGN(converter);
                explicit converter();
                virtual ~converter() throw();
                friend class singleton<converter>;
                tree_type tree;


            };


        }
    }
}

#endif

