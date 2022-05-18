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

        class blur : public article
        {
        public:
            class factor
            {
            public:
                factor() throw();
                factor(const factor &) throw();
                ~factor() throw();
                const coord  r;
                const float  w;
                friend std::ostream & operator<<(std::ostream &, const factor &);
                
            private:
                YACK_DISABLE_ASSIGN(factor);
            };

            virtual ~blur() throw();
            static  size_t inside(const unit_t r) throw();

            const float  sigma;
            const float  denom;
            const unit_t radius;
            const unit_t radsqr;

        protected:
            explicit blur(const float sig);

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(blur);
        };


        class blurring : public blur, public cxx_array<blur::factor,memory_allocator>
        {
        public:
            explicit blurring(const float sig);
            virtual ~blurring() throw();

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

