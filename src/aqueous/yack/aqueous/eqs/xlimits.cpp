
#include "yack/aqueous/eqs/xlimits.hpp"

namespace yack
{
    namespace aqueous
    {
        xlimits:: ~xlimits() noexcept
        {
        }

        xlimits:: xlimits() :
        sp_proxy( new sp_zpool() ),
        reac( *this ),
        prod( *this )
        {
        }

        limitation xlimits:: operator()(const components       &eq,
                                        const readable<double> &C0)
        {
            if( reac.get_extent(eq.reac,C0))
            {
                // limited by reac
                if(prod.get_extent(eq.prod,C0))
                {
                    // and limited by prod
                    return limited_by_both;
                }
                else
                {
                    // but not limited by prod
                    return limited_by_reac;
                }
            }
            else
            {
                // not limited by reac
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
