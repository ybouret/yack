//! \file

#ifndef YACK_CHEM_OUTCOME_INCLUDED
#define YACK_CHEM_OUTCOME_INCLUDED 1

#include "yack/chem/components.hpp"

namespace yack
{
    namespace chemical
    {
        struct extent
        {
            enum grade
            {
                is_significant,
                is_degenerated
            };
        };

        class outcome
        {
        public:
            outcome(const components::state s,
                    const extent::grade     g,
                    const double            x) throw();
            ~outcome() throw();
            outcome(const outcome &) throw();

            const components::state state; //!< state of components
            const extent::grade     grade; //!< grade of extent
            const double            value; //!< extent value

            static outcome study(const components       &comp,
                                 const double            K,
                                 const readable<double> &Cini,
                                 writable<double>       &Cend,
                                 rmulops                &ops);

        private:
            YACK_DISABLE_ASSIGN(outcome);
        };

    }
}

#endif
