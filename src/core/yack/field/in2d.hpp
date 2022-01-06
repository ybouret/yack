

//! \file

#ifndef YACK_FIELD2D_INCLUDED
#define YACK_FIELD2D_INCLUDED 1

#include "yack/field/in1d.hpp"
#include "yack/type/v2d.hpp"

namespace yack
{

    typedef v2d<unit_t>     coord2D;
    typedef layout<coord2D> layout2D;

    template <typename T>
    class field2D : public layout2D, public field_of<T>
    {
    public:
        YACK_DECL_ARGS_(T,type);
        typedef layout2D   layout_type;
        typedef layout1D   row_layout;
        typedef field1D<T> row_type;

        inline virtual ~field2D() throw() {}

        template <typename ID>
        explicit field2D(const ID         &I,
                         const layout_type L,
                         row_type         *r=0,
                         mutable_type     *p=0) :
        layout_type(L),
        field_of<T>(I),
        rplan(lower.x,upper.x)
        {
            setup(r,p);
        }

        const row_layout rplan;

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

