
namespace yack
{
    namespace math
    {
        template <> correlation<real_t>::correlation() throw() :
        points()
        {
        }

        template <> correlation<real_t>:: ~correlation() throw()
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
                arrays_of<real_t> tab(3,n);
                writable<real_t> &xx = tab.next();
                writable<real_t> &yy = tab.next();
                writable<real_t> &xy = tab.next();

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
                    const real_t xa = sorted::sum(xx,sorted::by_abs_value);
                    const real_t ya = sorted::sum(yy,sorted::by_abs_value);

                    for(size_t i=n;i>0;--i)
                    {
                        const real_t dx = xx[i]-xa;
                        const real_t dy = yy[i]-ya;
                        xy[i] = dx * dy;
                        xx[i] = dx*dx;
                        yy[i] = dy*dy;
                    }
                }

                const real_t sum_xy = sorted::sum(xy,sorted::by_abs_value);
                const real_t sum_x2 = sorted::sum(xx,sorted::by_value);
                const real_t sum_y2 = sorted::sum(yy,sorted::by_value);


                return sum_xy/( sqrt(sum_x2*sum_y2) + tiny );
            }
        }

    }

}
