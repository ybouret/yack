
#include "yack/chem/components.hpp"

namespace yack
{
    namespace chemical
    {

        components:: components() throw() :
        reac(),
        prod(),
        d_nu(0),
        db()
        {

        }

        components:: ~components() throw()
        {
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

        bool components:: add(const species &sp,
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


        std::ostream &components:: display(std::ostream &os) const
        {
            reac.display(os);
            os << " <=> ";
            prod.display(os);
            return os;
        }

    }
}
