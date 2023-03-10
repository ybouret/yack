
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

        const components_db::tree_type & components:: operator->() const noexcept
        {
            return cdb.tree;
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

        static const char rightleft[] = " <=> ";

        std::ostream & operator<<(std::ostream &os, const components &self)
        {
            os << self.reac << rightleft << self.prod;
            return os;
        }

        string components:: to_string() const
        {
            return reac.to_string() + rightleft + prod.to_string();
        }


        double components:: mass_action(const readable<double> &C,
                                        const double            K,
                                        cameo::mul<double>     &xmul)
        {
            assert(K>0);
            xmul.free();
            xmul.push(K);
            reac.mass_action(C,xmul);
            const double rhs = xmul.product();

            xmul.push(1);
            prod.mass_action(C,xmul);
            return rhs - xmul.product();
        }
        
    }
}

