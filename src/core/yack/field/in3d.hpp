
//! \file

#ifndef YACK_FIELD3D_INCLUDED
#define YACK_FIELD3D_INCLUDED 1

#include "yack/field/in2d.hpp"
#include "yack/type/v3d.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    // 3D types
    //__________________________________________________________________________
    typedef v3d<unit_t>     coord3D;  //!< coordinate
    typedef layout<coord3D> layout3D; //!< layout

    //__________________________________________________________________________
    //
    //
    //! 3D field
    //
    //__________________________________________________________________________
    template <typename T>
    class field3D : public layout3D, public field_of<T>
    {
    public:
        //______________________________________________________________________
        //
        // types
        //______________________________________________________________________
        YACK_DECL_ARGS_(T,type);         //!< aliases
        typedef layout3D   layout_type;  //!< alias
        typedef field1D<T> row_type;     //!< alias
        typedef field2D<T> slice_type;   //!< alias

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________

        //! cleanup
        inline virtual ~field3D() throw()
        {

        }

        //! setup
        template <typename ID>
        explicit field3D(const ID         &I,
                         const layout_type L)
        {
        }


    private:
        YACK_DISABLE_COPY_AND_ASSIGN(field3D);


    };

}

#endif
