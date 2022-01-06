//! \file

#ifndef YACK_FIELD_INCLUDED
#define YACK_FIELD_INCLUDED 1

#include "yack/string.hpp"
#include "yack/memory/embed.hpp"

namespace yack
{

    class field
    {
    public:
        virtual ~field() throw();


        const string &key() const throw();


        const string name;



    protected:
        template <typename ID> inline
        explicit field(const ID &id) :
        name(id),
        wksp(0),
        wlen(0)
        {
        }

        void  *wksp;
        size_t wlen;

        void   allocate(memory::embed emb[], const size_t num);


    private:
        YACK_DISABLE_COPY_AND_ASSIGN(field);

    };

    template <typename T>
    class field_of : public field
    {
    public:
        YACK_DECL_ARGS_(T,type);

        inline virtual ~field_of() throw() {}

    protected:
        template <typename ID>
        inline explicit field_of(const ID     &id) :
        field(id)
        {
        }
        

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(field_of);
    };

   

}

#endif

