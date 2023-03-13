
#include "yack/aqueous/eqs/components.hpp"
#include "yack/system/imported.hpp"
#include "yack/type/abs.hpp"
#include "yack/arith/gcd.h"

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

            assert(0==xmul.size());
            xmul.push(1);
            prod.mass_action(C,xmul);
            return rhs - xmul.product();
        }

        double components:: mass_action(const readable<double> &C,
                                        const double            xi,
                                        const double            K,
                                        cameo::mul<double>     &xmul)
        {
            assert(K>0);
            xmul.free();
            xmul.push(K);
            reac.mass_action(C,-xi,xmul);
            const double rhs = xmul.product();

            assert(0==xmul.size());
            xmul.push(1);
            prod.mass_action(C,xi,xmul);
            return rhs - xmul.product();
        }


        bool components:: is_neutral() const noexcept
        {
            int z = 0;
            for(const cnode *node= (*this)->head; node; node=node->next)
            {
                const component &cc = ***node;
                const species   &sp = *cc;
                const int        nu = cc.nu;
                z += nu * sp.z;
            }
            return z==0;
        }

        bool components:: is_minimal() const noexcept
        {
            const cnode *node= (*this)->head;
            if(node)
            {
                uint64_t g = absolute( (***node).nu );
                for(node=node->next;node;node=node->next)
                {
                    g = yack_gcd64(g, absolute( (***node).nu ));
                }
                return 1 == g;
            }
            else
            {
                // no node
                return true;
            }
        }

        
    }
}

