
//! \file

#ifndef YACK_FIELD1D_INCLUDED
#define YACK_FIELD1D_INCLUDED 1

#include "yack/field/field.hpp"
#include "yack/field/layout.hpp"
#include "yack/type/destruct.hpp"

namespace yack
{

    typedef unit_t          coord1D;
    typedef layout<coord1D> layout1D;

    template <typename T>
    class field1D : public layout1D, public field_of<T>
    {
    public:
        YACK_DECL_ARGS_(T,type);
        typedef layout1D layout_type;

        inline virtual ~field1D() throw() { item+=lower; clear(width); }

        template <typename ID>
        explicit field1D(const ID         &I,
                         const layout_type L,
                         mutable_type     *p=NULL) :
        layout_type(L),
        field_of<T>(I),
        item(0)
        {
            setup_with(p);
        }

        inline type & operator[](const unit_t x) throw()
        {
            assert(x>=lower); assert(x<=upper); return item[x];
        }

        inline const_type & operator[](const unit_t x) const throw()
        {
            assert(x>=lower); assert(x<=upper); return item[x];
        }


    private:
        YACK_DISABLE_COPY_AND_ASSIGN(field1D);
        mutable_type *item; //!< [lower:upper]

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

    };

}

#endif

