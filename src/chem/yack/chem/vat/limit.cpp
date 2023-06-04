
#include "yack/chem/vat/limit.hpp"


namespace yack
{
    namespace Chemical
    {
        Limit:: ~Limit() noexcept
        {
        }

        Limit:: Limit(const Species::Fund &sf) noexcept :
        Extended::Real(0),
        Species::CoopRepo(sf)
        {
        }

        void Limit:: initialize() noexcept
        {
            Extended::Real &xi = *this;
            clear();
            xi = 0;
        }

        std::ostream & operator<<(std::ostream &os, const Limit &lim)
        {
            const Species::CoopRepo &self = lim;
            switch(lim.size)
            {
                case 0:  os << "[none]"; break;
                default: os << Extended::Recv(lim) << " @" << self;
            }
            return os;
        }
    }

}
