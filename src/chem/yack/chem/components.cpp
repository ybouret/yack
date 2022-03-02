
#include "yack/chem/components.hpp"

namespace yack
{
    namespace chemical
    {

        components:: components() throw() :
        reac(),
        prod(),
        d_nu(0),
        db(),
        wksp()
        {

        }

        components:: ~components() throw()
        {
            for(const cnode *node=head();node;node=node->next)
            {
                const species &s = ****node;
                --coerce(s.rank);
            }
        }

        const cnode * components:: head() const throw()
        {
            return (*db.tree).head;
        }

        size_t components:: size() const throw()
        {
            return db.size();
        }

        void components:: update() throw()
        {
            coerce(d_nu) = prod.nu() - reac.nu();
        }

        bool components:: operator()(const species &sp,
                                     const int      nu)
        {
            assert(nu!=0);
            const component::pointer p = new component(sp,nu);
            if(!db.insert(p)) return false;

            try
            {
                if(nu>0)
                {
                    coerce(prod).push_back( new actor(sp,nu) );
                }
                else
                {
                    assert(nu<0);
                    coerce(reac).push_back( new actor(sp,-nu) );
                }
            }
            catch(...)
            {
                db.remove(sp.name);
                throw;
            }

            ++coerce(sp.rank);

            update();

            return true;
        }

        double components:: mass_action(const double K, const readable<double> &C) const throw()
        {
            return reac.mass_action(K,C) - reac.mass_action(1.0,C);
        }

        double components:: mass_action(const double K, const readable<double> &C, const double xi) const throw()
        {
            return reac.mass_action(K,C,-xi) - reac.mass_action(1.0,C,xi);
        }


        std::ostream &components:: display(std::ostream &os) const
        {
            reac.display(os);
            os << " <=> ";
            prod.display(os);
            return os;
        }

        const limits & components:: private_limits(const readable<double> &C) const throw()
        {
            return * new( YACK_STATIC_ZSET(wksp) ) limits( reac.private_limit(C), prod.private_limit(C) );
        }


    }
}
