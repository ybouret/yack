
#include "yack/aqueous/components.hpp"
#include "yack/system/imported.hpp"

namespace yack
{
    namespace aqueous
    {
        components:: ~components() noexcept
        {
        }

        components:: components() noexcept :
        large_object(),
        counted(),
        reac(),
        prod(),
        cdb()
        {
        }

        components:: components(const components &other) :
        large_object(),
        counted(),
        reac( other.reac ),
        prod( other.prod ),
        cdb(  other.cdb  )
        {

        }

        const char * const components::clid = "components";

        void components:: operator()(const int nu, const species &sp)
        {
            assert(nu!=0);
            if( cdb.search(sp.name) ) throw imported::exception(clid,"multiple [%s]", sp.name());
            const component::ptr cm = new component(sp,nu);
            actors              *ac = 0;
            if(nu>0)
            {

                (ac = &coerce(prod))->push_back(new actor(sp,static_cast<unsigned>(nu)));
            }
            else
            {
                (ac = &coerce(reac))->push_back(new actor(sp,static_cast<unsigned>(-nu)));
            }

            try
            {
                if(!cdb.insert(cm)) throw imported::exception(clid,"failure to insert %d[%s]",nu,sp.name());
            }
            catch(...)
            {
                delete ac->pop_back();
                throw;
            }

        }

        std::ostream & operator<<(std::ostream &os, const components &self)
        {
            os << self.reac << " <=> " << self.prod;
            return os;
        }

        
    }
}

