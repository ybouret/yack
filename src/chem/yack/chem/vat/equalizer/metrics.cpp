
#include "yack/chem/vat/equalizer/metrics.hpp"

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
            os << "valid: " << self.regulating << " | wrong: " << self.equalizing;
            return os;
        }

        void Metrics:: computeFrom(const readable<Extended::Real>    &C0,
                                   const Actors                      &actors,
                                   const readable<bool>              &regular,
                                   const IndexLevel                   level)
        {
            regulating.initialize();
            equalizing.clear();
            for(const Actor *ac=actors.head;ac;ac=ac->next)
            {
                const Species        &sp = **ac; if( ! regular[ sp.indx[SubLevel] ] ) continue;
                const Extended::Real &c0 = C0[ sp.indx[level] ];
                if(c0.m>=0)
                {
                    regulating.tryDecreaseWith(c0,*ac);
                }
                else
                {
                    equalizing.tryInsertCursor(-c0,*ac);
                }
            }
        }

        void Metrics:: computeFrom(const readable<Extended::Real>    &C0,
                                   const Actors                      &actors,
                                   const IndexLevel                   level)
        {
            regulating.initialize();
            equalizing.clear();
            for(const Actor *ac=actors.head;ac;ac=ac->next)
            {
                const Species        &sp = **ac;
                const Extended::Real &c0 = C0[ sp.indx[level] ];
                if(c0.m>=0)
                {
                    regulating.tryDecreaseWith(c0,*ac);
                }
                else
                {
                    equalizing.tryInsertCursor(-c0,*ac);
                }
            }
        }


        


    }

}
