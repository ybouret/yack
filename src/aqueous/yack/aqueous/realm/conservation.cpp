#include "yack/aqueous/realm/conservation.hpp"


namespace yack
{
    namespace aqueous
    {
        conservation:: ~conservation() noexcept
        {
        }

        conservation:: conservation() noexcept :
        object(), actors_(), next(0), prev(0)
        {
        }

        std::ostream & operator<<(std::ostream       &os,
                                const conservation &self)
        {
            os << '(';
            const actor *a = self.head;
            if(a)
            {
                a->display_first(os);
                for(a=a->next;a;a=a->next)
                {
                    a->display_extra(os);
                }
            }
            os << ')';
            return os;
        }

        bool conservation:: overlaps(const conservation &rhs) const noexcept
        {
            for(const actor *l=head;l;l=l->next)
            {
                const species &L = **l;
                for(const actor *r=rhs.head;r;r=r->next)
                {
                    const species &R = **r;
                    if( &L == &R ) return true;
                }
            }
            return false;
        }

    }
}


namespace yack
{
    namespace aqueous
    {
        conserved:: ~conserved() noexcept
        {
        }
    }

}

