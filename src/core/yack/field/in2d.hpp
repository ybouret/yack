

//! \file

#ifndef YACK_FIELD2D_INCLUDED
#define YACK_FIELD2D_INCLUDED 1

#include "yack/field/in1d.hpp"
#include "yack/type/v2d.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    // 2D types
    //__________________________________________________________________________
    typedef v2d<unit_t>     coord2D;  //!< coordinate
    typedef layout<coord2D> layout2D; //!< layout

    //__________________________________________________________________________
    //
    //
    //! 2D field
    //
    //__________________________________________________________________________
    template <typename T>
    class field2D : public layout2D, public field_of<T>
    {
    public:
        //______________________________________________________________________
        //
        // types
        //______________________________________________________________________
        YACK_DECL_ARGS_(T,type);         //!< aliases
        typedef layout2D   layout_type;  //!< alias
        typedef layout1D   row_layout;   //!< alias
        typedef field1D<T> row_type;     //!< alias

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________

        //! cleanup
        inline virtual ~field2D() throw() {}

        //! setup
        template <typename ID>
        explicit field2D(const ID         &I,
                         const layout_type L,
                         row_type         *r=0,
                         mutable_type     *p=0) :
        layout_type(L),
        field_of<T>(I)
        {
            setup(r,p);
        }

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(field2D);
        row_type *row;

        void setup(row_type *r, mutable_type *p)
        {
            if(!r)
            {
                assert(!p);
            }
            

        }

    };

}

#endif

