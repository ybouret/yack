//! \file

#ifndef YACK_GFX_BLUR_INCLUDED
#define YACK_GFX_BLUR_INCLUDED 1

#include "yack/gfx/pixmap.hpp"
#include "yack/sequence/cxx-array.hpp"
#include "yack/color/opaque.hpp"
#include "yack/gfx/memory.hpp"

namespace yack
{
    namespace graphic
    {
        //______________________________________________________________________
        //
        //
        //! blur metrics
        //
        //______________________________________________________________________
        class blur : public article
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            //! blur factor: pos + weight
            class factor
            {
            public:
                factor()               throw(); //!< zero
                factor(const factor &) throw(); //!< copy
                ~factor()              throw(); //!< cleanup
                friend std::ostream & operator<<(std::ostream &, const factor &); //!< display

                const coord  r; //!< relative pos
                const float  w; //!< weight

            private:
                YACK_DISABLE_ASSIGN(factor);
            };

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            static  size_t inside(const unit_t r) throw(); //!< count points x^2+y^2<=r2


            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const float  sigma;   //!< gaussian
            const float  denom;   //!< 2 * sigma^2
            const unit_t radius;  //!< matching min factor = 1/256
            const unit_t radsqr;  //!< radius*radius

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~blur() throw();        //!< cleanup

        protected:
            explicit blur(const float sig); //!< setup metrics

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(blur);
        };

        //______________________________________________________________________
        //
        //
        //! blurring weights and methods
        //
        //______________________________________________________________________
        class blurring : public blur, public cxx_array<blur::factor,memory_allocator>
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit blurring(const float sig); //!< setup
            virtual ~blurring() throw();        //!< cleanup

            //__________________________________________________________________
            //
            //! apply to scalar or vectors
            //__________________________________________________________________
            template <typename PIXEL, typename T, const size_t N> inline
            void apply(PIXEL &out, const pixmap<PIXEL> &source, const coord &pos) const
            {
                static const float opq = static_cast<float>(color::opaque<T>::value);
                float              acc[4] = {0,0,0,0};
                for(size_t i=size();i>0;--i)
                {
                    const factor f = (*this)[i];
                    const PIXEL &p = source[pos+f.r];
                    const T     *q = (const T *)&p;
                    for(size_t k=0;k<N;++k)
                    {
                        acc[k] += static_cast<float>(q[k]) * f.w;
                    }
                }

                {
                    T *q = (T *)&out;
                    for(size_t k=0;k<N;++k)
                    {
                        q[k] = static_cast<T>( clamp<float>(0,acc[k],opq) );
                    }
                }
            }

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(blurring);
        };
    }
}

#endif

