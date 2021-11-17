//! \file

#ifndef YACK_ASSOCIATIVE_GLOSSARY_INCLUDED
#define YACK_ASSOCIATIVE_GLOSSARY_INCLUDED 1

#include "yack/container/associative.hpp"

namespace yack
{

    template <typename KEY, typename T>
    class glossary : public associative<KEY,T>
    {
    public:
        YACK_DECL_ARGS(T,type);
        YACK_DECL_ARGS(KEY,key_type);

        inline virtual ~glossary() throw() {}

        virtual bool insert(param_key_type,param_type) = 0;

    protected:
        inline explicit glossary() throw() {}

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(glossary);
    };

}

#endif

