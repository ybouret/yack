
#include "yack/aqueous/eqs/xlimits.hpp"

namespace yack
{
    namespace aqueous
    {

        const char * xlimits:: text(const limitation l) noexcept
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

        xlimits:: ~xlimits() noexcept
        {
        }

        xlimits:: xlimits() :
        sp_proxy( new sp_zpool() ),
        reac( *this ),
        prod( *this ),
        flag( are_blocked )
        {
        }

        limitation xlimits:: operator()(const components       &eq,
                                        const readable<double> &C0)
        {
            flag = are_running;
            if( reac.get_extent(eq.reac,C0))
            {
                // limited by reac
                if(prod.get_extent(eq.prod,C0))
                {
                    // and limited by prod
                    if( fabs(prod.xi) <=0 && fabs(reac.xi) <=0 ) flag = are_blocked;
                    return limited_by_both;
                }
                else
                {
                    // but not limited by prod => are_running
                    return limited_by_reac;
                }
            }
            else
            {
                // not limited by reac => are_running
                if(prod.get_extent(eq.prod,C0))
                {
                    // but limited by prod
                    return limited_by_prod;
                }
                else
                {
                    // and not limited by prod
                    return limited_by_none;
                }
            }
        }
        

    }

}
