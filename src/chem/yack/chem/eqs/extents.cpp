
#include "yack/chem/eqs/extents.hpp"

namespace yack
{
    namespace Chemical
    {

        const char *LimitationText(const Limitation l) noexcept
        {
            switch(l)
            {
                case LimitedByReac: return "reac";
                case LimitedByProd: return "prod";
                case LimitedByBoth: return "both";
                case LimitedByNone:
                    break;
            }
            return "none";
        }


        Extents:: ~Extents() noexcept
        {
        }

        Extents:: Extents(const Species::Fund &speciesFund ) noexcept:
        reac(speciesFund),
        prod(speciesFund),
        kind(LimitedByNone)
        {
        }

        void Extents:: reset() noexcept
        {
            coerce(reac).clear();
            coerce(prod).clear();
            coerce(kind) = LimitedByNone;
        }

        static inline
        void updateRegulating(Regulating                     &regulating,
                              const Actors                   &actors,
                              const readable<Extended::Real> &C0,
                              const IndexLevel                level)
        {
            for(const Actor *ac=actors.head;ac;ac=ac->next)
            {
                const Species       &sp = **ac;
                const size_t         sj = sp.indx[level];
                const Extended::Real cj = C0[sj]; assert(cj>=0);
                regulating.tryDecreaseWith(cj,*ac);
            }
        }

        void Extents:: build(const Components               &eq,
                             const readable<Extended::Real> &C0,
                             const IndexLevel                level)
        {
            reset();
            try
            {
                updateRegulating(coerce(reac),eq.reac,C0,level);
                updateRegulating(coerce(prod),eq.prod,C0,level);
                updateLimitation();
            }
            catch(...)
            {
                reset();
                throw;
            }
        }

        void Extents:: updateLimitation() noexcept
        {
            if(reac.size)
            {
                // reac.size > 0
                if(prod.size>0)
                {
                    coerce(kind) = LimitedByBoth;
                }
                else
                {
                    coerce(kind) = LimitedByReac;
                }
            }
            else
            {
                // reac.size = 0
                if(prod.size>0)
                {
                    coerce(kind) = LimitedByProd;
                }
                else
                {
                    coerce(kind) = LimitedByNone;
                }
            }
        }


        std::ostream & Extents:: display(std::ostream &os) const
        {

            os << "limited by ";

            switch(kind)
            {
                case LimitedByNone: os << LimitationText(kind); break;

                case LimitedByReac: os << LimitationText(kind); os << ' ' << reac; break;

                case LimitedByProd: os << LimitationText(kind); os << ' ' << prod; break;

                case LimitedByBoth:
                    os << LimitationText(LimitedByReac) << ' ' << reac;
                    os << " and ";
                    os << LimitationText(LimitedByProd) << ' ' << prod;
                    break;
            }

            return os;
        }

    }

}
