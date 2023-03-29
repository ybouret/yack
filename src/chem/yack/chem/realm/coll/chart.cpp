
#include "yack/chem/realm/coll/chart.hpp"

namespace yack
{
    namespace chemical
    {
        namespace collecting
        {

            chart:: ~chart() noexcept
            {
            }

            chart:: chart(const caches &cc) noexcept :
            reac(cc.zlp,cc.spp),
            prod(cc.zlp,cc.spp),
            corr(cc.spp)
            {
            }

            chart::oor_type chart:: settle(const components       &E,
                                           const readable<double> &C,
                                           const readable<bool>   &R)
            {

                if(reac.honored_by(E.reac,C,R))
                {
                    // valid reac
                    if( prod.honored_by(E.prod,C,R) )
                    {
                        // valid prod
                        return oor_none;
                    }
                    else
                    {
                        // out of range prod only
                        return oor_prod;
                    }
                }
                else
                {
                    // out of range reac
                    if( prod.honored_by(E.prod,C,R) )
                    {
                        // but valid prod
                        return oor_reac;
                    }
                    else
                    {
                        // out of range prod as well
                        return oor_both;
                    }
                }

            }

            shift_status chart:: adjust_prod(const xmlog &xml)
            {
                return prod.oor.try_shift(xml,corr,reac.lim);
            }

            shift_status chart:: adjust_reac(const xmlog &xml)
            {
                const shift_status res = reac.oor.try_shift(xml,corr,prod.lim);

                switch(res)
                {
                    case shift_blocked: return shift_blocked;
                    case shift_partial:
                    case shift_success:
                        corr.extent = -corr.extent;
                        return res;
                }
                return res;
                
            }




            std::ostream & operator<<(std::ostream &os, const chart &self)
            {
                os <<   self.reac << " <=> " << self.prod;
                return os;
            }

            
        }
    }
}

