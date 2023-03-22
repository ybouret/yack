
#include "yack/aqueous/realm/coll/chart.hpp"

namespace yack
{
    namespace aqueous
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

            bool chart:: adjust_prod(const xmlog &xml)
            {
                return prod.oor.lookup(xml,corr,reac.lim);
            }

            bool chart:: adjust_reac(const xmlog &xml)
            {
                if(reac.oor.lookup(xml,corr,prod.lim))
                {
                    corr.extent = -corr.extent;
                    return true;
                }
                else
                {
                    return false;
                }
            }




            std::ostream & operator<<(std::ostream &os, const chart &self)
            {
                os <<   self.reac << " <=> " << self.prod;
                return os;
            }

            
        }
    }
}

