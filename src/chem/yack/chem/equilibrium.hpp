

#ifndef YACK_CHEM_EQ_INCLUDED
#define YACK_CHEM_EQ_INCLUDED 1

#include "yack/chem/actor.hpp"
#include "yack/chem/components.hpp"
#include "yack/type/gateway.hpp"

namespace yack
{

    namespace chemical
    {

        class equilibrium : public object, public counted
        {
        public:
            virtual ~equilibrium() throw();

            template <typename ID> inline
            explicit equilibrium(const ID &id) :
            name(id),
            reac(),
            prod(),
            comp()
            {
            }

            void add(const species &sp, const unit_t nu);

            const string name;
            const actors reac;
            const actors prod;

        private:
            const components comp;
        };

    }

}

#endif
