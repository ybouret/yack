
#include "yack/chem/eqs/zlimit.hpp"

namespace yack
{
    namespace chemical
    {

        zlimit:: ~zlimit() noexcept
        {
        }

        zlimit:: zlimit(const sp_proxy &proxy) noexcept :
        sp_repo(proxy),
        extent(0)
        {
        }

        zlimit:: zlimit(const zlimit &other) :
        sp_repo(other),
        extent(other.extent)
        {
            assert(other.size==size);
        }

        void zlimit:: trade(zlimit &other) noexcept
        {
            cswap(extent,other.extent);
            swap_with(other);
        }

        zlimit & zlimit:: operator=(const zlimit &other)
        {
            zlimit temp(other);
            trade(temp);
            return *this;
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

        void zlimit:: nullify(writable<double> &C) const noexcept
        {
            for(const sp_node *zn=head;zn;zn=zn->next)
            {
                C[ (***zn).indx[top_level] ] = 0;
            }
        }

     }

}
