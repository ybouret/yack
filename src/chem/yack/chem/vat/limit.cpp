
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

        Limit:: Limit(const Limit &other) :
        Extended::Real(other),
        Species::CoopRepo(other)
        {
        }

        static inline void xr_assign(Extended::Real       &target,
                                     const Extended::Real &source) noexcept
        {
            target = source;
        }

        void Limit:: neg() noexcept
        {
            Extended::Real &xi = *this;
            xi = -xi;
        }

        Limit & Limit:: operator=(const Limit &other)
        {
            Limit tmp(other);
            swap_with(tmp);
            xr_assign(*this,other);
            return *this;
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
                default: os << Extended::Recv(lim) << "@" << self;
            }
            return os;
        }
    }

}
