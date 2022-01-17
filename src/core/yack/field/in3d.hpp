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
        YACK_DECL_ARGS_(T,type);                 //!< aliases
        typedef layout3D            layout_type; //!< alias
        typedef coord3D             coord_type;  //!< alias
        typedef loop_on<coord_type> loop_type;   //!< alias
        typedef field1D<T>          row_type;    //!< alias
        typedef field2D<T>          slice_type;  //!< alias
        static const size_t dimensions = 3;      //!< static dimensions

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________

        //! cleanup
        inline virtual ~field3D() throw()
        {
            slice += lower.z;
            clear(width.z);
        }

        //! setup
        template <typename ID>
        explicit field3D(const ID         &I,
                         const layout_type L,
                         slice_type       *s=0,
                         row_type         *r=0,
                         mutable_type     *p=0) :
        layout_type(L),
        field_of<T>(I,dimensions),
        slice(0)
        {
            setup(s,r,p);
        }

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________

        //! access
        inline slice_type & operator[](const unit_t z) throw()
        {
            assert(slice);assert(z>=lower.z); assert(z<=upper.z); return slice[z];
        }

        //! access, const
        inline const slice_type & operator[](const unit_t z) const throw()
        {
            assert(slice);assert(z>=lower.z); assert(z<=upper.z); return slice[z];
        }

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(field3D);
        slice_type *slice;

        inline void clear(unit_t done) throw()
        {
            while(done>0)
            {
                destruct( &slice[--done] );
            }
        }

        inline void setup(slice_type       *s,
                          row_type         *r,
                          mutable_type     *p)
        {
            if(!s)
            {
                assert(!p); assert(!r);
                memory::embed emb[] =
                {
                    memory::embed(s,width.z),
                    memory::embed(r,width.z*width.y),
                    memory::embed(p,items)
                };
                this->allocate(emb,sizeof(emb)/sizeof(emb[0]));
            }

            assert(s); assert(r); assert(p);
            // link
            unit_t done = 0;
            unit_t indx = lower.z;
            slice       = s;
            try
            {
                const layout2D l2( lower.xy(), upper.xy() );
                const size_t   dp = l2.items;
                const size_t   dr = width.y;
                while(done<width.z)
                {
                    const string id = this->name + vformat("[%ld]", (long)indx );
                    new (s+done) slice_type(id,l2,r,p);
                    ++done;
                    ++indx;
                    p += dp;
                    r += dr;
                }
            }
            catch(...)
            {
                clear(done);
                throw;
            }
            slice -= lower.z;

        }

        //! read
        const_type & fetch(const readable<unit_t> &coord) const throw()
        {
            assert(coord.size()>=3);
            return (*this)[coord[3]][coord[2]][coord[1]];
        }


    };

}

#endif
