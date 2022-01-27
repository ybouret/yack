#include "yack/chem/equilibrium.hpp"
#include "yack/exception.hpp"

namespace yack
{

    namespace chemical
    {

        equilibrium:: ~equilibrium() throw()
        {
        }

        void equilibrium:: add(const species &sp, const unit_t nu)
        {
            const string     &spid = sp.name;
            const component  &c    = coerce(comp).create(sp,nu);
            actors           *a    = 0;

            switch( __sign::of(nu) )
            {
                case __zero__: throw exception("<%s> invalid nu=0 for '%s'", name(), spid() );
                case negative: a = & coerce(reac); break;
                case positive: a = & coerce(prod); break;
            }

            assert(NULL!=a);
            try
            {
                a->push_back( new actor(c) );
            }
            catch(...)
            {
                (void) coerce(comp).remove(spid);
                throw;
            }

        }

    }

}

