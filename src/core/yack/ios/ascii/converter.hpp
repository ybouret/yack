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

            //__________________________________________________________________
            //
            //
            //! builder procedure for a given type
            //
            //__________________________________________________________________
            typedef void (*builder)(void *,const char *,const size_t,const char *);


            //__________________________________________________________________
            //
            //
            //! database to convert ints, reals and string
            //
            //__________________________________________________________________
            class converter : public singleton<converter>
            {
            public:
                //______________________________________________________________
                //
                // types and definition
                //______________________________________________________________
                static const char                            call_sign[];       //!< ascii::converter
                static const at_exit::longevity              life_time = 10000; //!< longevity
                typedef kernel::suffix_tree<builder,uint8_t> tree_type;         //!< alias

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________
                builder        & operator[](const rtti &) const; //!< find builder for rtti
                static builder & of(const rtti &);               //!< global find builder for rtti

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

