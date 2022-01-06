//! \file

#ifndef YACK_FIELD_INCLUDED
#define YACK_FIELD_INCLUDED 1

#include "yack/string.hpp"
#include "yack/memory/embed.hpp"
#include "yack/container/dynamic.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! base class for fields
    //
    //__________________________________________________________________________
    class field : public dynamic
    {
    public:
        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        const string  &key()     const throw(); //!< for sets
        virtual size_t granted() const throw(); //!< wlen

        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
        const string name; //!< identifier

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        virtual ~field() throw(); //!< cleanup

    protected:
        //! setup
        template <typename ID> inline
        explicit field(const ID &id) :
        name(id),
        wksp(0),
        wlen(0)
        {
        }


        //! allocated memory in private workspace
        void   allocate(memory::embed emb[], const size_t num);


    private:
        YACK_DISABLE_COPY_AND_ASSIGN(field);
        void  *wksp;
        size_t wlen;

    };


    //__________________________________________________________________________
    //
    //
    //! base class for typed fields
    //
    //__________________________________________________________________________
    template <typename T>
    class field_of : public field
    {
    public:
        //______________________________________________________________________
        //
        // types
        //______________________________________________________________________
        YACK_DECL_ARGS_(T,type); //!< aliases


        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________

        //! cleanup
        inline virtual ~field_of() throw() {}

    protected:

        //! setup
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

