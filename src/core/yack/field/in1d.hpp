
//! \file

#ifndef YACK_FIELD1D_INCLUDED
#define YACK_FIELD1D_INCLUDED 1

#include "yack/field/field.hpp"
#include "yack/field/layout.hpp"
#include "yack/type/destruct.hpp"
#include "yack/counting/loop-on.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    // 1D types
    //__________________________________________________________________________
    typedef unit_t          coord1D;   //!< coordinate
    typedef layout<coord1D> layout1D;  //!< layout

    //__________________________________________________________________________
    //
    //
    //! 1D field
    //
    //__________________________________________________________________________
    template <typename T>
    class field1D : public layout1D, public field_of<T>
    {
    public:
        //______________________________________________________________________
        //
        // types
        //______________________________________________________________________
        YACK_DECL_ARGS_(T,type);                 //!< aliases
        typedef layout1D            layout_type; //!< alias
        typedef coord1D             coord_type;  //!< alias
        typedef loop_on<coord_type> loop_type;   //!< alias
        static const size_t dimensions = 1;      //!< static dims

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________

        //! cleanup
        inline virtual ~field1D() throw() { item+=lower; clear(width); }

        //! setup
        template <typename ID>
        explicit field1D(const ID         &I,
                         const layout_type L,
                         mutable_type     *p=NULL) :
        layout_type(L),
        field_of<T>(I,dimensions),
        item(0)
        {
            setup_with(p);
        }

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________

        //! access
        inline type & operator[](const unit_t x) throw()
        {
            assert(item); assert(x>=lower); assert(x<=upper); return item[x];
        }

        //! access
        inline const_type & operator[](const unit_t x) const throw()
        {
            assert(item); assert(x>=lower); assert(x<=upper); return item[x];
        }

        

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(field1D);
        mutable_type *item; //!< [lower:upper]

        //! setup data
        inline void setup_with(mutable_type *p)
        {
            if(p)
            {
                item = p;
            }
            else
            {
                memory::embed emb[] =
                {
                    memory::embed(item,width)
                };
                this->allocate(emb,sizeof(emb)/sizeof(emb[0]));
            }

            {
                unit_t done = 0;
                try
                {
                    while(done<width)
                    {
                        new (item+done) mutable_type();
                        ++done;
                    }
                }
                catch(...)
                {
                    clear(done);
                    throw;
                }
            }

            item -= lower;
        }

        // clear
        inline void clear(unit_t n) throw()
        {
            while(n>0)
            {
                destruct(&item[--n]);
            }
        }

        //! read
        const_type & fetch(const readable<unit_t> &coord) const throw()
        {
            assert(coord.size()>=1);
            return (*this)[coord[1]];
        }

    };

}

#endif

