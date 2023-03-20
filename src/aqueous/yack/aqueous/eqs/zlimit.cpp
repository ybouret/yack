
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
