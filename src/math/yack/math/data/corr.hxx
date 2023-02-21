
namespace yack
{
    namespace math
    {
        template <> correlation<real_t>::correlation() noexcept :
        points(),
        xadd()
        {
        }

        template <> correlation<real_t>:: ~correlation() noexcept
        {
        }

        template <>
        correlation<real_t> & correlation<real_t>:: operator()(const real_t x, const real_t y)
        {
            (*this) << point(x,y);
            return *this;
        }


        template <>
        real_t correlation<real_t>:: operator()(void)
        {
            static const real_t tiny(1e-30);

            const size_t n = size();
            if(n<=2)
            {
                return 1;
            }
            else
            {
                arrays_of<real_t> tb(3,n);
                writable<real_t> &xx = tb.next();
                writable<real_t> &yy = tb.next();
                writable<real_t> &xy = tb.next();

                // fill xx with p.x, yy with p.y
                {
                    size_t i=1;
                    for(const node_type *node=head();node;node=node->next,++i)
                    {
                        const point p = **node;
                        xx[i] = p.x;
                        yy[i] = p.y;
                    }
                }


                {
                    const real_t xa = xadd.tableau(xx)/n; // x-average
                    const real_t ya = xadd.tableau(yy)/n; // y-average

                    for(size_t i=n;i>0;--i)
                    {
                        const real_t dx = xx[i]-xa;
                        const real_t dy = yy[i]-ya;
                        xy[i] = dx*dy;
                        xx[i] = dx*dx;
                        yy[i] = dy*dy;
                    }
                }

                const real_t sum_xy = xadd.tableau(xy);
                const real_t sum_x2 = xadd.tableau(xx);
                const real_t sum_y2 = xadd.tableau(yy);


                return sum_xy/( std::sqrt(sum_x2*sum_y2) + tiny );
            }
        }

    }

}
