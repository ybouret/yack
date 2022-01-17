//! \file

#ifndef YACK_FIELD4D_INCLUDED
#define YACK_FIELD4D_INCLUDED 1

#include "yack/field/in3d.hpp"
#include "yack/type/v4d.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    // 4D types
    //__________________________________________________________________________
    typedef v4d<unit_t>     coord4D;  //!< coordinate
    typedef layout<coord4D> layout4D; //!< layout

    //__________________________________________________________________________
    //
    //
    //! 4D field
    //
    //__________________________________________________________________________
    template <typename T>
    class field4D : public layout4D, public field_of<T>
    {
    public:
        //______________________________________________________________________
        //
        // types
        //______________________________________________________________________
        YACK_DECL_ARGS_(T,type);            //!< aliases
        typedef layout4D   layout_type;     //!< alias
        typedef coord4D    coord_type;      //!< alias
        typedef field1D<T> row_type;        //!< alias
        typedef field2D<T> slice_type;      //!< alias
        typedef field3D<T> volume_type;     //!< alias

        static const size_t dimensions = 4; //!< static dimensions

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________

        //! cleanup
        inline virtual ~field4D() throw()
        {
            volume += lower.w;
            clear(width.w);
        }

        //! setup
        template <typename ID>
        explicit field4D(const ID         &I,
                         const layout_type L ) :
        layout_type(L),
        field_of<T>(I,dimensions),
        volume(0)
        {
            setup();
        }

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________

        //! access
        inline volume_type & operator[](const unit_t w) throw()
        {
            assert(volume);assert(w>=lower.w); assert(w<=upper.w); return volume[w];
        }

        //! access, const
        inline const volume_type & operator[](const unit_t w) const throw()
        {
            assert(volume);assert(w>=lower.w); assert(w<=upper.w); return volume[w];
        }

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(field4D);
        volume_type *volume;

        inline void clear(unit_t done) throw()
        {
            while(done>0)
            {
                destruct( &volume[--done] );
            }
        }

        inline void setup()
        {
            slice_type   *s = 0;
            row_type     *r = 0;
            mutable_type *p = 0;

            const unit_t nv = width.w;
            const size_t ns = nv * width.z;
            const size_t nr = ns * width.y;
            memory::embed emb [] =
            {
                memory::embed(volume,nr),
                memory::embed(s,ns),
                memory::embed(r,nr),
                memory::embed(p,items)
            };

            this->allocate(emb,sizeof(emb)/sizeof(emb[0]));
            unit_t done = 0;
            unit_t indx = lower.w;
            try
            {
                const layout3D l3( lower.xyz(), upper.xyz() );
                const size_t   ds = width.z;         // slices per volume
                const size_t   dr = width.z*width.y; // rows per volume
                const size_t   dp = l3.items;        // items per volume
                while(done<nv)
                {
                    const string id = this->name + vformat("[%ld]", (long)indx );
                    new (volume+done) volume_type(id,l3,s,r,p);
                    ++done;
                    ++indx;
                    s += ds;
                    r += dr;
                    p += dp;
                }

            }
            catch(...)
            {
                clear(done);
                throw;
            }

            volume -= lower.w;

        }

    };

}

#endif
