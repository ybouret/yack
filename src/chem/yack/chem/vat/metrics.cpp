
#include "yack/chem/vat/metrics.hpp"

namespace yack
{
    namespace Chemical
    {

        Metrics:: ~Metrics() noexcept
        {
        }

        Metrics:: Metrics(const Species::Fund &spcFund,
                          const Cursor::Fund  &crsFund) noexcept :
        regulating(spcFund),
        equalizing(spcFund,crsFund)
        {
        }

        std::ostream & operator<<(std::ostream &os, const Metrics &self)
        {
            os << self.regulating << " | " << self.equalizing;
            return os;
        }

        void Metrics:: computeFrom(const readable<Extended::Real>    &C0,
                                   const Actors                      &actors,
                                   const readable<Species::Category> &category,
                                   const IndexLevel                   level)
        {
            regulating.initialize();
            equalizing.clear();
            for(const Actor *ac=actors.head;ac;ac=ac->next)
            {
                const Species &sp = **ac;
                if( Species::Unbounded == category[sp.indx[SubLevel]] ) continue;
                const Extended::Real &c = C0[ sp.indx[level] ];
                if(c.m>=0)
                {
                    regulating.tryDecreaseWith(c,*ac);
                }
                else
                {
                    equalizing.tryInsertCursor(-c,*ac);
                }
            }

        }

    }

}

