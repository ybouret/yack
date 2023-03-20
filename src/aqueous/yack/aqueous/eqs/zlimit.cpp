
#include "yack/aqueous/eqs/zlimit.hpp"

namespace yack
{
    namespace aqueous
    {

        zlimit:: ~zlimit() noexcept
        {
        }

        zlimit:: zlimit(const sp_proxy &proxy) noexcept :
        sp_repo(proxy),
        extent(0)
        {
        }

        zlimit:: zlimit(const sp_proxy &pp,
                        const double    xi,
                        const species  &sp) :
        sp_repo(pp),
        extent(xi)
        {
            (*this) << sp;
        }

        void zlimit:: reset() noexcept
        {
            extent = 0;
            clear();
        }


        std::ostream & operator<<(std::ostream &os, const zlimit &self)
        {
            const sp_repo &repo = self;
            os << repo << " @" << self.extent;
            return os;
        }
    }

}
