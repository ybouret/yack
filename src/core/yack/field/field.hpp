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
        const size_t dims; //!< dimension

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        virtual ~field() throw(); //!< cleanup

    protected:
        //! setup
        template <typename ID> inline
        explicit field(const ID &id, const size_t nd) :
        name(id),
        dims(nd),
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


        //______________________________________________________________________
        //
        // interface
        //______________________________________________________________________

        //! access by [x,y,z,w]
        inline const_type & operator()(const readable<unit_t> &coord) const throw()
        {
            return fetch(coord);
        }

        //! access by [x,y,z,w]
        inline type & operator()(const readable<unit_t> &coord) throw()
        {
            return (type&)fetch(coord);
        }

    protected:

        //! setup
        template <typename ID>
        inline explicit field_of(const ID &id, const size_t nd) :
        field(id,nd)
        {
        }
        

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(field_of);

        virtual const_type & fetch(const readable<unit_t> &coord) const throw() = 0;

    };

   

}

#endif

