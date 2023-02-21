#include "yack/gfx/blur.hpp"
#include "yack/type/utils.hpp"
#include "yack/sort/heap.hpp"
#include "yack/comparison.hpp"
#include <cmath>
#include <iostream>

namespace yack
{
    namespace graphic
    {

        blur :: ~blur() noexcept {}

        static inline unit_t compute_radius(const float sigma)
        {
            assert(sigma>=0);
            const unit_t r = static_cast<unit_t>(ceil(sigma * 4.0f * sqrt( logf(2.0f) )));
            return r<=1 ? 1 : r;
        }

        size_t blur:: inside(const unit_t r) noexcept
        {
            assert(r>=0);
            const unit_t r2 = r*r;
            size_t       in = 0;
            for(unit_t y=-r;y<=r;++y)
            {
                const unit_t y2 = y*y;
                for(unit_t x=-r;x<=r;++x)
                {
                    if(y2+x*x<=r2)
                    {
                        ++in;

                    }
                }
            }
            return in;
        }


        blur:: blur(const float sig) :
        sigma( std::abs(sig) ),
        denom( twice(squared(sigma) ) ),
        radius( compute_radius(sigma) ),
        radsqr( squared(radius) )
        {


        }

        blur::factor::  factor() noexcept : r(), w(0) {}
        blur::factor:: ~factor() noexcept {}
        blur:: factor:: factor(const factor &f) noexcept :
        r(f.r), w(f.w)
        {
        }

        std::ostream & operator<<(std::ostream &os, const blur::factor &f)
        {
            os << "(" << f.r.x << "," << f.r.y << ")=" << f.w;
            return os;
        }

        blurring  :: ~blurring()  noexcept {}


        static inline int compare_blur_factors(const blur::factor &lhs,
                                               const blur::factor &rhs) noexcept
        {
            return comparison::decreasing(lhs.w,rhs.w);
        }

        blurring:: blurring(const float sig) :
        blur(sig),
        cxx_array<blur::factor,memory_allocator>(inside(radius))
        {

            const unit_t r  = radius;
            const unit_t r2 = radsqr;
            size_t       in = 0;
            for(unit_t y=-r;y<=r;++y)
            {
                const unit_t y2 = y*y;
                for(unit_t x=-r;x<=r;++x)
                {
                    const unit_t d2 = y2+x*x;
                    if(d2<=r2)
                    {
                        ++in;
                        const factor &f = (*this)[in];
                        coerce(f.r) = coord(x,y);
                        coerce(f.w) = expf( -d2/denom );
                    }
                }
            }

            hsort(*this,compare_blur_factors);
            float sum = 0;
            for(size_t i=size();i>0;--i) sum += (*this)[i].w;
            for(size_t i=size();i>0;--i) coerce( (*this)[i].w )/=sum;
        };

    }
}

