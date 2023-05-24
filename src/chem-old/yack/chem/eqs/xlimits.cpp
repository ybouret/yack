
#include "yack/chem/eqs/xlimits.hpp"

namespace yack
{
    namespace chemical
    {

        const char * xlimits:: limit_text(const limitation l) noexcept
        {
            switch(l)
            {
                case limited_by_none: return "none";
                case limited_by_prod: return "prod";
                case limited_by_reac: return "reac";
                case limited_by_both: return "both";
            }
            return yack_unknown;
        }

        const char * xlimits:: avail_text(const availability a) noexcept
        {
            switch(a)
            {
                case is_blocked: return "[blocked]";
                case is_running: return "[running]";
            }
            return yack_unknown;
        }

        xlimits:: ~xlimits() noexcept
        {
        }

        xlimits:: xlimits() :
        sp_proxy( new sp_zpool() ),
        reac( *this ),
        prod( *this ),
        type( limited_by_none ),
        flag( is_blocked )
        {
        }

        limitation xlimits:: operator()(const index_level       I,
                                        const components       &E,
                                        const readable<double> &C)
        {
            flag = is_running;
            if( reac.search(I,E.reac,C))
            {
                // limited by reac
                if(prod.search(I,E.prod,C))
                {
                    // and limited by prod
                    if( fabs(prod.extent) <=0 && fabs(reac.extent) <=0 ) flag = is_blocked;
                    return (type=limited_by_both);
                }
                else
                {
                    // but not limited by prod => are_running
                    return (type=limited_by_reac);
                }
            }
            else
            {
                // not limited by reac => are_running
                if(prod.search(I,E.prod,C))
                {
                    // but limited by prod
                    return (type=limited_by_prod);
                }
                else
                {
                    // and not limited by prod
                    return (type=limited_by_none);
                }
            }
        }

        limitation xlimits:: operator()(const components       &E,
                                        const readable<double> &C,
                                        const readable<bool>   &R)
        {
            flag = is_running;
            if( reac.search(E.reac,C,R))
            {
                // limited by reac
                if(prod.search(E.prod,C,R))
                {
                    // and limited by prod
                    if( fabs(prod.extent) <=0 && fabs(reac.extent) <=0 ) flag = is_blocked;
                    return (type=limited_by_both);
                }
                else
                {
                    // but not limited by prod => are_running
                    return (type=limited_by_reac);
                }
            }
            else
            {
                // not limited by reac => are_running
                if(prod.search(E.prod,C,R))
                {
                    // but limited by prod
                    return (type=limited_by_prod);
                }
                else
                {
                    // and not limited by prod
                    return (type=limited_by_none);
                }
            }
        }


        std::ostream & operator<<(std::ostream &os, const xlimits &self)
        {
            os << self.avail_text(self.flag) << " limited by " << self.limit_text(self.type) << " : ";

            switch(self.type)
            {
                case limited_by_none: break;
                case limited_by_reac: os << self.reac; break;
                case limited_by_prod: os << self.prod; break;
                case limited_by_both: os << "reac: " << self.reac << " | prod: " << self.prod; break;
            }
            return os;
        }

    }

}
