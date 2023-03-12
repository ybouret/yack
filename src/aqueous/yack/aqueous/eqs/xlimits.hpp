
//! \file

#ifndef YACK_AQUEOUS_XLIMITS_INCLUDED
#define YACK_AQUEOUS_XLIMITS_INCLUDED 1

#include "yack/aqueous/eqs/xlimit.hpp"
#include "yack/aqueous/eqs/components.hpp"

namespace yack
{
    namespace aqueous
    {
        enum limitation
        {
            limited_by_none,
            limited_by_reac,
            limited_by_prod,
            limited_by_both
        };

        class xlimits : public sp_proxy
        {
        public:
            explicit xlimits();
            virtual ~xlimits() noexcept;

            xlimit reac;
            xlimit prod;

            limitation operator()(const components       &,
                                  const readable<double> &);



        private:
            YACK_DISABLE_COPY_AND_ASSIGN(xlimits);
        };
    }

}

#endif

