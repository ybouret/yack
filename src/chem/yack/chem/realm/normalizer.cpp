
#include "yack/aqueous/realm/normalizer.hpp"

namespace yack
{
    namespace aqueous
    {

        normalizer:: ~normalizer() noexcept
        {
        }

        normalizer:: normalizer(const realm &chem) :
        guard( chem.lib->size ),
        slide( chem.largest_domain_size(), chem.lib->size )
        {
        }
        
        void normalizer:: operator()(const realm      &chem,
                                     writable<double> &conc)
        {
            coerce(guard).process(chem,conc);
            coerce(slide).balance(chem,conc);
        }

    }

}

