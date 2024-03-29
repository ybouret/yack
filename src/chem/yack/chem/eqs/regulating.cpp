
#include "yack/chem/eqs/regulating.hpp"


namespace yack
{
    namespace Chemical
    {
        Regulating:: Regulating(const Species::Fund &sf) noexcept :
        Limit(sf)
        {
        }

        Regulating:: ~Regulating() noexcept
        {
        }

        bool Regulating:: isBlocking() const noexcept
        {
            return m<=0;
        }

      

        void Regulating:: tryDecreaseWith(const Extended::Real &c, const Actor &a)
        {
            Extended::Real      &xi  = *this;
            const Extended::Real tmp = c/a.xn;
            if(size<=0)
            {
                xi      = tmp;
                (*this) << *a;
            }
            else
            {
                switch( Extended::Comp(tmp,xi) )
                {
                    case negative:
                        clear();
                        xi = tmp;
                        (*this) << *a;
                        break;

                    case __zero__:
                        (*this) << *a;
                        break;

                    case positive:
                        break;
                }
            }
        }

    }

}


