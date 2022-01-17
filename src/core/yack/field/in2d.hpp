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
        YACK_DECL_ARGS_(T,type);            //!< aliases
        typedef layout2D   layout_type;     //!< alias
        typedef coord2D    coord_type;      //!< alias
        typedef field1D<T> row_type;        //!< alias
        static const size_t dimensions = 2; //!< static dims

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________

        //! cleanup
        inline virtual ~field2D() throw()
        {
            row += lower.y;
            clear(width.y);
        }

        //! setup
        template <typename ID>
        explicit field2D(const ID         &I,
                         const layout_type L,
                         row_type         *r=0,
                         mutable_type     *p=0) :
        layout_type(L),
        field_of<T>(I,dimensions),
        row(0)
        {
            setup(r,p);
        }

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________

        //! access
        inline row_type & operator[](const unit_t y) throw()
        {
            assert(row); assert(y>=lower.y); assert(y<=upper.y); return row[y];
        }

        //! access
        inline const row_type & operator[](const unit_t y) const throw()
        {
            assert(row); assert(y>=lower.y); assert(y<=upper.y); return row[y];
        }



    private:
        YACK_DISABLE_COPY_AND_ASSIGN(field2D);
        row_type *row; //!< in [lower.y:upper.y]

        //! erase memory
        inline void clear(unit_t done) throw()
        {
            while(done>0)
                destruct( &row[--done] );
        }

        void setup(row_type     *r,
                   mutable_type *p)
        {
            const size_t nr = size_t(width.y);

            //------------------------------------------------------------------
            //
            // check memory
            //
            //------------------------------------------------------------------
            if(!r)
            {
                assert(!p);
                memory::embed emb[] =
                {
                    memory::embed(r,nr),
                    memory::embed(p,items)
                };
                this->allocate(emb,sizeof(emb)/sizeof(emb[0]));
            }

            assert(r);
            assert(p);

            //------------------------------------------------------------------
            //
            // create rows
            //
            //------------------------------------------------------------------
            row         = r;
            unit_t irow = lower.y;
            try
            {
                const size_t   step = width.x;
                const layout1D rlay(lower.x,upper.x);
                while(irow<=upper.y)
                {
                    const string rtag = vformat("[%ld]",static_cast<long>(irow));
                    const string ruid = this->name + rtag;
                    new (r) row_type(ruid,rlay,p);
                    ++irow;
                    ++r;
                    p += step;
                }
                assert(irow-lower.y==width.y);
            }
            catch(...)
            {
                clear(irow-lower.y);
                throw;
            }
            row -= lower.y;
        }

    };

}

#endif

