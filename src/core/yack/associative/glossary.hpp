//! \file

#ifndef YACK_ASSOCIATIVE_GLOSSARY_INCLUDED
#define YACK_ASSOCIATIVE_GLOSSARY_INCLUDED 1

#include "yack/container/associative.hpp"

namespace yack
{

    //__________________________________________________________________________
    //
    //
    //! glossary (key,type)
    //
    //__________________________________________________________________________
    template <typename KEY, typename T>
    class glossary : public associative<KEY,T>
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        YACK_DECL_ARGS(T,type);          //!< aliases
        YACK_DECL_ARGS(KEY,key_type);    //!< aliases


        //______________________________________________________________________
        //
        //! insert key+value
        //______________________________________________________________________
        virtual bool insert(param_key_type,param_type) = 0;

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline virtual ~glossary() throw() {} //!< cleanup
    protected:
        inline explicit glossary() throw() {} //!< setup

        //! multipurpose display
        template <typename OSTREAM, typename NODE> static inline
        OSTREAM &display(OSTREAM &os, const NODE *node)
        {
            os << '{';
            if(node)
            {
                node->disp(os);
                while(NULL!=(node=node->next))
                {
                    node->disp(os<<',');
                }
            }
            os << '}';
            return os;
        }


    private:
        YACK_DISABLE_COPY_AND_ASSIGN(glossary);


    };

}

#endif

