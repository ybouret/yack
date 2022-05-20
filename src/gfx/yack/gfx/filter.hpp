
//! \file

#ifndef YACK_GFX_FILTER_INCLUDED
#define YACK_GFX_FILTER_INCLUDED 1

#include "yack/gfx/pixmap.hpp"
#include "yack/gfx/rgb.hpp"
#include "yack/gfx/coord.hpp"
#include "yack/gfx/memory.hpp"
#include "yack/string.hpp"
#include "yack/type/utils.hpp"

namespace yack
{
    namespace graphic
    {

        //______________________________________________________________________
        //
        //
        //! compiled filter
        //
        //______________________________________________________________________
        class filter : public article
        {
        public:
            //__________________________________________________________________
            //
            //  types and definitions
            //__________________________________________________________________
            typedef float real_t; //!< for internal computation

            //! way to build coordinates
            enum building
            {
                direct, //!< (x,y)
                rotate  //!< (y,x)
            };

            //! C-style weight = coord+value
            class weight
            {
            public:
                weight() throw(); //!< all zero
                weight(const coord, const real_t) throw(); //!< setup
                weight(const weight &)            throw(); //!< copy
                ~weight()                         throw(); //!< cleanup
                friend std::ostream & operator<<(std::ostream &, const weight &); //!< display

                const coord  r; //!< relative position
                const real_t w; //!< value

            private:
                YACK_DISABLE_ASSIGN(weight);
            };

            //! alias
            typedef cxx_array<weight,memory_allocator> weights;

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual       ~filter() throw(); //!< cleanup


            //! generic setup
            template <typename FILTER_NAME,
            typename T> inline
            explicit filter(const FILTER_NAME &filter_name,
                            const T       *tab,
                            const unit_t   nx,
                            const unit_t   ny,
                            const building way) :
            name(filter_name),
            data(count(tab,nx,ny))
            {
                fill(tab,nx,ny,way);
            }

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            const string & key() const throw(); //!< for pointer
            friend std::ostream & operator<<(std::ostream &, const filter &); //!< display

            //! apply local filter
            template <typename T> inline
            real_t operator()(const pixmap<T> &source, const coord pos) const throw()
            {
                real_t res = 0;
                for(size_t k=data.size();k>0;--k)
                {
                    const weight w = data[k];
                    res += static_cast<real_t>( source[w.r+pos] ) * w.w;
                }
                return res;
            }
            
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const string  name; //!< identifier
            const weights data; //!< weights

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(filter);
            template <typename T>
            static inline size_t count(const T *tab, const unit_t nx, const unit_t ny)
            {
                size_t res = 0;
                assert(nx>0);
                assert(ny>0);
                for(unit_t j=0;j<ny;++j)
                {
                    const T *row = &tab[j*nx];
                    for(unit_t i=0;i<nx;++i)
                    {
                        if( absolute(row[i]) > 0 ) ++res;
                    }
                }
                return res;
            }

            template <typename T>
            inline void fill(const T *tab, const unit_t nx, const unit_t ny, const building way)
            {
                size_t res = 0;
                const unit_t x_off = -nx/2;
                const unit_t y_off = -ny/2;
                for(unit_t j=0;j<ny;++j)
                {
                    const T *row = &tab[j*nx];
                    for(unit_t i=0;i<nx;++i)
                    {
                        const T value = row[i];
                        if( absolute(value) > 0 )
                        {
                            const weight &w = data[++res];
                            switch(way)
                            {
                                case direct:
                                    coerce(w.r.x) = x_off+i;
                                    coerce(w.r.y) = y_off+j;
                                    break;

                                case rotate:
                                    coerce(w.r.y) = x_off+i;
                                    coerce(w.r.x) = y_off+j;
                                    break;
                            }
                            coerce(w.w)   = static_cast<real_t>(value);
                        }
                    }
                }
            }

        };

    }

}

#endif

